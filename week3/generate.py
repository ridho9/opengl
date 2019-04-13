import sys

points = {}

print("#ifndef __CAR_")
print("#define __CAR_")

verticesCount = 0

print("float vertices[] = {")

while True:
    try:
        line: str = input().strip()
    except EOFError:
        break

    if line.startswith("#") or len(line) == 0:
        continue

    line_split = line.split()

    if len(line_split) == 4:
        # create point
        id = line_split[0]
        points[id] = [float(_) for _ in line_split[1:]]
        sys.stderr.write(f"point [{id}] = {points[id]}\n")
    elif len(line_split) == 6:
        # create triangle
        id = line_split[:3]
        color = [float(_) for _ in line_split[3:]]

        id = map(lambda x: points[x], id)
        for point in id:
            for v in point:
                print(f"{v}, ", end="")
            print(f"{color[0]}, {color[1]}, {color[2]}, ")
            verticesCount += 1

        sys.stderr.write(f"triangle {id}, {color}\n")
    else:
        sys.stderr.write(f"Invalid line '{line}'\n")
        break

print("};")

print(f"int verticesCount = {verticesCount};")
print("#endif")
