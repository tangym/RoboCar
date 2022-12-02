import sys
import argparse


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('command', type=str, nargs='*')
    args = parser.parse_args()

    if args.command:
        print(f"{args.command} is executed.")
    else:
        command = input()
        while command.lower() not in ['q', 'quit', 'exit']:
            print(f"{command} is executed.")
            command = input()
sys.exit(0)
