D:\Pro\Release\ReconstructMesh scene_dense.mvs --remove-spurious 0 --remove-spikes 0 --close-holes 0 --smooth 0 -d 0 --export 1 --free-space-support 0 -o scene_dense_mesh_base_t.mvs -w "F:\Downloads\DTU\Points\3dn\run6"
start "" /B runEval.cmd 6 scene_dense_mesh_base_t.ply
D:\Pro\Release\ReconstructMesh scene_dense.mvs --remove-spurious 0 --remove-spikes 0 --close-holes 0 --smooth 0 -d 0 --export 1 --free-space-support 1 -o scene_dense_mesh_fs_t.mvs -w "F:\Downloads\DTU\Points\3dn\run6"
start "" /B runEval.cmd 6 scene_dense_mesh_fs_t.ply
