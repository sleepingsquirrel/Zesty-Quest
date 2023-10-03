import os
for file in os.listdir("scenes/"):
    print(file)
    os.system(f'mkdir out/{file.split(".")[0]}')
    os.system(f'ffmpeg -i "{"scenes/" + file}" -vf fps=24/1 "out/{file.split(".")[0]}/out-%03d.jpg"')
