from client import RoboCar
import config


def main():
    controller = None
    client = RoboCar()
    while True:
        controller = input("input (w/s/x or q): ")
        client.containers['controller'].post(controller)
        if controller == "q":
            break


if __name__ == "__main__":
    main()
