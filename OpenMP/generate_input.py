import random
import string
import sys

def generate_random_string(length):
    letters = string.ascii_letters
    return ''.join(random.choice(letters) for i in range(length))

def main():
    if len(sys.argv) < 3:
        print("Usage: python generate.py <length of string> <output filename>")
        return 1

    length = int(sys.argv[1])
    output_filename = sys.argv[2]

    random_string = generate_random_string(length)

    with open(output_filename, 'w') as f:
        f.write(random_string)

if __name__ == "__main__":
    main()
