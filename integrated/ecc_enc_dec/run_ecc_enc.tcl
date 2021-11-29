open_project hls_ecc_enc_dec_prj -reset
add_files  src/ecc_enc_dec_tb.cpp
add_files -tb src/ecc_enc_dec_tb.cpp
set_top ecc_enc_dec

open_solution "solution0" -reset
#set_part {xc7z020clg484-1}
set_part {xczu9eg-ffvb1156-2-e}
create_clock -period 5 -name default
csim_design -ldflags "-lcrypto -lssl"
csynth_design 
cosim_design -ldflags "-lcrypto -lssl"
export_design -format ip_catalog
close_project
