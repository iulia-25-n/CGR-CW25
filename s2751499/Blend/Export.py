import bpy
import os

# writes into ASCII folder
out_path = bpy.path.abspath("//../ASCII/scene.txt")

# if folder exists
os.makedirs(os.path.dirname(out_path), exist_ok=True)

with open(out_path, 'w') as f:
    f.write("# Min ASCII scene for Module 1\n")

    cam = bpy.context.scene.camera
    if cam is None:
        print("No camera found in this scene.")
    else:
        loc = cam.location
        rot = cam.rotation_euler
        f.write("CAMERA\n")
        f.write(f"location {loc.x} {loc.y} {loc.z}\n")
        f.write(f"Euler {rot.x} {rot.y} {rot.z}\n")
        f.write(f"forward {cam.matrix_world.to_3x3() @ (0,0,-1)}\n")
        f.write(f"up {cam.matrix_world.to_3x3() @ (0,1,0)}\n")
        f.write("focal_sensor 50.0 36.0 24.0\n")
        f.write("resolution 1920 1080\n")

print(f"✅ Scene exported to {out_path}")
