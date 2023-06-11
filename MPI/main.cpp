#include <mpi/mpi.h>
#include <fstream>
#include <vector>

const int ITERATION_COUNT = 5;

const int ALIVE = 1;
const int DEAD = 0;

// for debugging
void print_board(const std::vector<int>& board, const int& rows, const int& rank) {
    const int cols = board.size() / rows;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << board[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << rank << std::endl;
    std::cout << std::endl;
}

std::pair<std::vector<int>, std::pair<int, int>> read_board(const int& rank) {
    std::vector<int> board;
    int rows, cols;
    if (rank == 0) {
        std::ifstream in("start.txt");
        in >> rows >> cols;
        board.resize(rows * cols);
        for (int i = 0; i < rows * cols; ++i) {
            in >> board[i];
        }
    }
    return std::make_pair(board, std::make_pair(rows, cols));
}

void initialize_processes(std::vector<int>& board, int& rows, int& cols, const int& rank) {
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        board = std::vector<int>(rows * cols);
    }
}

std::pair<std::vector<int>, std::vector<int>> initialize_sizes(const int& rows, const int& cols, const int& rank, const int& size) {
    std::vector<int> counts(size), starts(size);
    for (int i = 0; i < size; ++i) {
        counts[i] = (rows / size + (i < rows % size ? 1 : 0)) * cols;
        starts[i] = (i > 0 ? starts[i - 1] + counts[i - 1] : 0);
    }
    return std::make_pair(counts, starts);
}

void share_board(std::vector<int>& local_board, std::vector<int>& board, std::vector<int>& counts,
                 std::vector<int>& starts, const int& rows, const int& cols, const int& rank, const int& size) {
    MPI_Scatterv(board.data(), counts.data(), starts.data(), MPI_INT,
                 local_board.data(), counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
}

void share_halo_rows(std::vector<int>& local_board, std::vector<int>& board, std::vector<int>& counts,
                     std::vector<int>& starts, std::vector<int>& halo_up, std::vector<int>& halo_down,
                     const int& local_rows, const int& cols, const int& rank, const int& size) {
    if (rank == 0) {
        std::fill(halo_up.begin(), halo_up.end(), DEAD);
    }
    if (rank == size - 1) {
        std::fill(halo_down.begin(), halo_down.end(), DEAD);
    }

    // NOTE: it is important to have these ifs arranged like that to avoid deadlocks
    if (rank != 0) {
        MPI_Recv(halo_up.data(), cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (rank != size - 1) {
        MPI_Send(local_board.data() + (local_rows - 1) * cols, cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
    if (rank != size - 1) {
        MPI_Recv(halo_down.data(), cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (rank != 0) {
        MPI_Send(local_board.data(), cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    }
}

void collect_board(std::vector<int>& local_board, std::vector<int>& board, std::vector<int>& counts,
                   std::vector<int>& starts, const int& rows, const int& cols, const int& rank, const int& size) {
    if (rank == 0) {
        board.resize(rows * cols);
    }
    MPI_Gatherv(local_board.data(), counts[rank], MPI_INT,
                board.data(), counts.data(), starts.data(), MPI_INT, 0, MPI_COMM_WORLD);
}

void save_board(std::vector<int>& board, const int& rows, const int& cols, const int& rank) {
    if (rank == 0) {
        std::ofstream out("end.txt");
        out << rows << " " << cols << std::endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                out << board[i * cols + j] << " ";
            }
            out << std::endl;
        }
    }
}



int get(std::vector<int>& local_board, std::vector<int>& halo_up, std::vector<int>& halo_down, int i, int j, int rows, int cols) {
    if (i == -1) {
        return (j < 0 || j >= cols) ? DEAD : halo_up[j];
    } else if (i == rows) {
        return (j < 0 || j >= cols) ? DEAD : halo_down[j];
    } else {
        return (j < 0 || j >= cols || i < 0 || i > rows) ? DEAD : local_board[i * cols + j];
    }
}

void tick(std::vector<int>& local_board, std::vector<int>& halo_up, std::vector<int>& halo_down,
          const int& rows, const int& cols) {
    std::vector<int> new_board = local_board;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int alive_neighbors = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    alive_neighbors += get(local_board, halo_up, halo_down, i + di, j + dj, rows, cols);
                }
            }
            if (local_board[i * cols + j] == ALIVE) {
                new_board[i * cols + j] = (alive_neighbors == 2 || alive_neighbors == 3) ? ALIVE : DEAD;
            } else {
                new_board[i * cols + j] = (alive_neighbors == 3) ? ALIVE : DEAD;
            }
        }
    }
    local_board = new_board;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto configuration = read_board(rank);
    std::vector<int>& board = configuration.first;
    int rows = configuration.second.first;
    int cols = configuration.second.second;

    initialize_processes(board, rows, cols, rank);

    auto size_configuration = initialize_sizes(rows, cols, rank, size);
    std::vector<int>& counts = size_configuration.first;
    std::vector<int>& starts = size_configuration.second;

//    print_board(counts, 1, rank);
//    print_board(starts, 1, rank);

    std::vector<int> local_board(counts[rank]);
    share_board(local_board, board, counts, starts, rows, cols, rank, size);

    std::vector<int> halo_up(cols), halo_down(cols);

    int local_rows = counts[rank] / cols;


    for (int i = 0; i < ITERATION_COUNT; i++) {
//        std::cout << "HERE" << std::endl;
        share_halo_rows(local_board, board, counts, starts, halo_up, halo_down, local_rows, cols, rank, size);
//        std::cout << "HERE1" << std::endl;
//        print_board(halo_up, 1, rank);
//        print_board(halo_down, 1, rank);
        tick(local_board, halo_up, halo_down, local_rows, cols);
//        std::cout << "HERE2" << std::endl;
    }

//    print_board(board, rows, rank);

//    print_board(local_board, local_rows, rank);

    collect_board(local_board, board, counts, starts, rows, cols, rank, size);

//    print_board(board, rows, rank);

    save_board(board, rows, cols, rank);

    MPI_Finalize();

    return 0;
}
