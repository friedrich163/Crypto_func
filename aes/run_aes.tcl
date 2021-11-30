open_project hls_aes_prj -reset

add_files src/aescbc_tb.c
add_files src/aescbc.c
add_files src/aescbc.h
add_files src/aesecb.c
add_files src/aesecb.h
add_files -tb src/aescbc_tb.c
set_top aes

open_solution "solution0" -reset
#set_part {xc7z020clg484-1}
set_part {xczu9eg-ffvb1156-2-e}
create_clock -period 5 -name default
csim_design -ldflags "-lcrypto -lssl"
csynth_design 
cosim_design -ldflags "-lcrypto -lssl"
export_design -format ip_catalog
close_project

exit
