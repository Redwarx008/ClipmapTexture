import json

file = open("compile_commands.json", "r+")
data = json.load(file)
for entry in data:
    entry["directory"] = "E:\\Repos\\ClipmapTexture\\godot"
outdata = json.dumps(data, indent=4)

outfile = open("new.json", "w")
outfile.write(outdata)