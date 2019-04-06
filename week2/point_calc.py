#!/usr/bin/env python

IMAGE_WIDTH, IMAGE_HEIGHT = [int(v) for v in input().split()]

points = {}

print("#ifndef _VERTICES")
print("#define _VERTICES")

print("float vertices[] = {")

while True:
    line = input()

    if line.startswith("="):
        break

    line = line.split()
    if len(line) != 3:
        exit(-1)

    i, x, y = [int(v) for v in line]

    xp = x - (IMAGE_WIDTH / 2)
    yp = (IMAGE_HEIGHT / 2) - y
    xp = xp / (IMAGE_WIDTH / 2)
    yp = yp / (IMAGE_HEIGHT / 2)

    # print(f"{xp},{yp}")
    points[i] = (xp, yp)

# print(points)

# read triangle
count = 0
while True:
    try:
        ax, bx, cx, hex = [v for v in input().split()]

        r, g, b = tuple(int(hex[i : i + 2], 16) for i in (0, 2, 4))

        ax = int(ax)
        bx = int(bx)
        cx = int(cx)
        r /= 255
        g /= 255
        b /= 255
        color_string = f"{r : .5f}f, {g : .5f}f, {b : .5f}f,"

        def point_string(idx):
            return f"{points[idx][0] : .5f}f, {points[idx][1] : .5f}f, "

        print("    ", point_string(ax), color_string, "//")
        print("    ", point_string(bx), color_string, "//")
        print("    ", point_string(cx), color_string, "//")

        count += 1

    except EOFError:
        break

print("};")
print(f"int verticesCount = {count * 3};")
print("#endif")

print(f"// {count * 3}")
