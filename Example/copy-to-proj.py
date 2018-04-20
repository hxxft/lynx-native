import shutil, os

example_dir = os.path.split(os.path.realpath(__file__))[0] 
src_dir = example_dir + '/dist' 
manifest = example_dir + '/src/manifest.json'  
dist_dir_ios = example_dir + '/../iOS/assets.bundle' 
dist_dir_android = example_dir + '/../Android/example/src/main/assets'  

def clear_dir(dist_path):
    for i in os.listdir(dist_path):
         path_file = os.path.join(dist_path,i)
         if os.path.isfile(path_file):
            os.remove(path_file)
         else:
            clear_dir(path_file) 
            os.removedirs(path_file)

def pre_copy(dist_path):
    if os.path.exists(dist_path): 
        clear_dir(dist_path)
    if not os.path.exists(dist_path):
        os.mkdir(dist_path)

def do_copy(dist_path):
    dirs = os.listdir(src_dir)
    for dir in dirs:
        dist = dist_path + '/' + dir.lower() 
        path_file = os.path.join(src_dir, dir)
        shutil.copytree(path_file, dist) 
    shutil.copy(manifest, dist_path) 

def copy(dist_dir):
    pre_copy(dist_dir)
    do_copy(dist_dir)

copy(dist_dir_ios)
copy(dist_dir_android)