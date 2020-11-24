import cv2
import argparse
import os.path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('path')
    args = parser.parse_args()
    img = cv2.imread(args.path, 0)
    if not os.path.isfile(args.path) or img is None:
        print('Image not found')
        return
    filtered = cv2.Sobel(img, cv2.CV_8U, 1, 0, ksize=5)
    cv2.imwrite(args.path, filtered)


if __name__ == '__main__':
    main()
