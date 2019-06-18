#!/bin/sh
port_array="0 1 2 3"
disp_rgn="n"
port0_case=""
port1_case=""
port2_case=""
port3_case=""
src_fps=""
dest_fps=""
vifworkmode=""
vpeworkmode=""
vifwidth=""
vifheight=""
vpe_port0_w=""
vpe_port0_h=""
vpe_port1_w=""
vpe_port1_h=""
vpe_port2_w=""
vpe_port2_h=""
vpe_port3_w=""
vpe_port3_h=""
panel_dip_chn=5
panel_w=480
panel_h=864
do_panel_flow=0

start_rtsp() {
	./mdbenter integrate
	./mdbcmd vpecreateport 0 $1 $2 $3 11
	./mdbcmd vencinit $2 $3 $1 $4
	./mdbcmd vencstart $1
	./mdbcmd setbind 7 0 0 $1 $src_fps 2 0 $1 0 $dest_fps
	./mdbcmd rtspstart stream$1 $4
	./mdbexit
	case $1 in
	0)
		vpe_port0_w="$2"
		vpe_port0_h="$3"
	;;
	1)
		vpe_port1_w="$2"
		vpe_port1_h="$3"
	;;
	2)
		vpe_port2_w="$2"
		vpe_port2_h="$3"
	;;
	3)
		vpe_port3_w="$2"
		vpe_port3_h="$3"
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
}
start_rtsp_with_dip() {
	./mdbenter integrate
	./mdbcmd vpecreateport 0 $1 $2 $3 11
	./mdbcmd divpinit $1 0 0 0 0 0 0 0
	./mdbcmd divpcreateport $1 11 $2 $3
	read -p "DIVP/VENC miu sel(0/1):" select
	if [ $select -eq 1 ]; then
		mma_name="mma_heap_name1"
	else
		mma_name="NULL"
	fi
	./mdbcmd setmma 12 0 $1 $mma_name
	./mdbcmd divpstart $1
	./mdbcmd setbind 7 0 0 $1 $src_fps 12 0 $1 0 $dest_fps
	./mdbcmd vencinit $2 $3 $1 $4
	./mdbcmd setmma 2 0 $1 $mma_name
	./mdbcmd vencstart $1
	./mdbcmd setbind 12 0 $1 0 $src_fps 2 0 $1 0 $dest_fps
	./mdbcmd rtspstart stream$1 $4
	./mdbexit
	case $1 in
	0)
		vpe_port0_w="$2"
		vpe_port0_h="$3"
	;;
	1)
		vpe_port1_w="$2"
		vpe_port1_h="$3"
	;;
	2)
		vpe_port2_w="$2"
		vpe_port2_h="$3"
	;;
	3)
		vpe_port3_w="$2"
		vpe_port3_h="$3"
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
}
stop_rtsp() {
	./mdbenter integrate
	./mdbcmd rtspstop stream$1
	./mdbcmd setunbind  7 0 0 $1 2 0 $1 0
	./mdbcmd vencstop $1
	./mdbcmd vencdeinit $1
	./mdbcmd vpedestroyport $1 0
	./mdbexit
	case $1 in
	0)
		vpe_port0_w=""
		vpe_port0_h=""
	;;
	1)
		vpe_port1_w=""
		vpe_port1_h=""
	;;
	2)
		vpe_port2_w=""
		vpe_port2_h=""
	;;
	3)
		vpe_port3_w=""
		vpe_port3_h=""
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
}
stop_rtsp_with_dip() {
	./mdbenter integrate
	./mdbcmd rtspstop stream$1
	./mdbcmd setunbind 12 0 $1 0 2 0 $1 0
	./mdbcmd vencstop $1
	./mdbcmd vencdeinit $1
	./mdbcmd setunbind 7 0 0 $1 12 0 $1 0
	./mdbcmd divpstop $1
	./mdbcmd divpdeinit $1
	./mdbcmd vpedestroyport $1 0
	./mdbexit
	case $1 in
	0)
		vpe_port0_w=""
		vpe_port0_h=""
	;;
	1)
		vpe_port1_w=""
		vpe_port1_h=""
	;;
	2)
		vpe_port2_w=""
		vpe_port2_h=""
	;;
	3)
		vpe_port3_w=""
		vpe_port3_h=""
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
}
create_vpe_for_panel() {
	./mdbenter integrate
	echo "Type 0 for Panel display yuv422."
	echo "Type 1 for Panel display yuv420sp."
	echo "Type 'Enter' to ignore."
	read -p "Please select:" select
	case $select in
	0)
		format=0
		do_panel_flow=1
	;;
	1)
		format=11
		do_panel_flow=1
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
	./mdbcmd vpecreateport 0 $1 $panel_w $panel_h $format
	./mdbexit
}
do_panel_start() {
	format=11
	case $1 in
	0)
		if [ "$vpe_port0_w" == "" ] && [ "$vpe_port0_h" == "" ]; then
			create_vpe_for_panel $1
		fi
	;;
	1)
		if [ "$vpe_port1_w" == "" ] && [ "$vpe_port1_h" == "" ]; then
			create_vpe_for_panel $1
		fi
	;;
	2)
		if [ "$vpe_port2_w" == "" ] && [ "$vpe_port2_h" == "" ]; then
			create_vpe_for_panel $1
		fi
	;;
	3)
		if [ "$vpe_port3_w" == "" ] && [ "$vpe_port3_h" == "" ]; then
			create_vpe_for_panel $1
		fi
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
	./mdbenter integrate
	./mdbcmd divpinit $panel_dip_chn 0 0 0 0 0 0 0
	./mdbcmd divpcreateport $panel_dip_chn $format $panel_w $panel_h
	./mdbcmd divpstart $panel_dip_chn
	./mdbcmd setbind 7 0 0 $1 $src_fps 12 0 $panel_dip_chn 0 $dest_fps
	./mdbcmd panelon $panel_w $panel_h $format
	./mdbcmd setbind 12 0 $panel_dip_chn 0 $src_fps 15 0 0 0 $dest_fps
	./mdbexit
}
do_panel_stop() {
	./mdbenter integrate
	./mdbcmd setunbind 12 0 $panel_dip_chn $1 15 0 0 0
	./mdbcmd paneloff
	./mdbcmd setunbind 7 0 0 $1 12 0 $panel_dip_chn 0
	./mdbcmd divpstop $panel_dip_chn
	./mdbcmd divpdeinit $panel_dip_chn
	case $1 in
	0)
		if [ "$vpe_port0_w" == "" ] && [ "$vpe_port0_h" == "" ]; then
			./mdbcmd vpedestroyport 0 0
		fi
	;;
	1)
		if [ "$vpe_port1_w" == "" ] && [ "$vpe_port1_h" == "" ]; then
			./mdbcmd vpedestroyport 1 0
		fi
	;;
	2)
		if [ "$vpe_port2_w" == "" ] && [ "$vpe_port2_h" == "" ]; then
			./mdbcmd vpedestroyport 2 0
		fi
	;;
	3)
		if [ "$vpe_port3_w" == "" ] && [ "$vpe_port3_h" == "" ]; then
			./mdbcmd vpedestroyport 3 0
		fi
	;;
	*)
		echo "Ignore !!!!"
		./mdbexit
		return
	;;
	esac
	do_panel_flow=0
	
	./mdbexit
}
do_rtsp_capture_jpeg() {
	./mdbenter integrate
	./mdbcmd vpecreateport 0 $1 $2 $3 11
	./mdbcmd vencinit $2 $3 $1 0
	./mdbcmd vencstart $1
	./mdbcmd setbind 7 0 0 $1 $src_fps 2 0 $1 0 $dest_fps
	./mdbcmd vencwritefile /mnt/capture$(date "+%Y%m%d-%H%M%S").jpg $1 0 1
	./mdbcmd setunbind 7 0 0 $1 2 0 $1 0
	./mdbcmd vencstop $1
	./mdbcmd vencdeinit $1
	./mdbcmd vpedestroyport $1 0
	./mdbexit
}
do_choice_port_on() {
	if [ "$1" == "0" ]; then
		port0_case="$2"
	fi
	if [ "$1" == "1" ]; then
		port1_case="$2"
	fi
	if [ "$1" == "2" ]; then
		port2_case="$2"
	fi
	if [ "$1" == "3" ]; then
		port3_case="$2"
	fi
}
do_choice_port_off() {
	do_divp_flow=""
	if [ "$1" == "0" ]; then
		do_divp_flow="$port0_case"
		port0_case=""
	fi
	if [ "$1" == "1" ]; then
		do_divp_flow="$port1_case"
		port1_case=""
	fi
	if [ "$1" == "2" ]; then
		do_divp_flow="$port2_case"
		port2_case=""
	fi
	if [ "$1" == "3" ]; then
		do_divp_flow="$port3_case"
		port3_case=""
	fi
	if [ "$do_divp_flow" == "0" ]; then
		stop_rtsp $1
	fi
	if [ "$do_divp_flow" == "1" ]; then
		stop_rtsp_with_dip $1
	fi
	if [ "$do_panel_flow" == "1" ]; then
		do_panel_stop $1
	fi
}

do_init() {
	./service &
	read -p "Display osd & cover [y/n]? >" disp_rgn
	./mdbinit
	if [ "$disp_rgn" == "y" ]; then
		./mdbenter region
		./mdbcmd init
		./mdbcmd create 0 0 3 200 200
		./mdbcmd create 1 0 3 200 200
		./mdbcmd create 2 1 0 0 0
		./mdbcmd create 3 1 0 0 0
		./mdbcmd create 4 1 0 0 0
		./mdbcmd create 5 1 0 0 0
		./mdbcmd setcanvas 0 14 3
		./mdbcmd setcanvas 1 14 3
		./mdbexit
	fi
}
do_deinit() {
	if [ "$disp_rgn" == "y" ]; then
		./mdbenter region
		./mdbcmd destroy 0
		./mdbcmd destroy 1
		./mdbcmd destroy 2
		./mdbcmd destroy 3
		./mdbcmd destroy 4
		./mdbcmd destroy 5
		./mdbcmd deinit
		./mdbexit
	fi
	./mdbdeinit
}
do_region_osd() {
	for port in $port_array
	do
		if [ "$disp_rgn" == "y" ]; then
			if [ "$port" == "2" ]; then
				./mdbenter region
				./mdbcmd attachosd 0 1 0 $port 0 1 0 0 0 0 0 0 0 0
				./mdbcmd attachosd 1 1 0 $port 0 1 28 200 1 0 0 0 0 0
				./mdbexit
			else
				./mdbenter region
				./mdbcmd attachosd 0 0 0 0 $port 1 0 0 0 0 0 0 0 0
				./mdbcmd attachosd 1 0 0 0 $port 1 28 200 1 0 0 0 0 0
				./mdbexit
			fi
		fi
	done
}
do_region_cover() {
	for port in $port_array
	do
		if [ "$disp_rgn" == "y" ]; then
			./mdbenter region
			./mdbcmd attachcover 2 0 0 0 $port 1 1000 1000 0 2000 2000 0xFF
			./mdbcmd attachcover 3 0 0 0 $port 1 1500 1500 1 2500 2500 0xFF00
			./mdbcmd attachcover 4 0 0 0 $port 1 2000 2000 2 3000 1000 0xFF0000
			./mdbcmd attachcover 5 0 0 0 $port 1 2500 2500 3 1000 2000 0x0
			./mdbexit
		fi
	done
}

do_rtsp_select() {
	echo "############################################"
	echo "Selected vpe--venc--rtsp case"
	echo "Starting config vpe port$1 and venc output size."
	echo "Type 0 for H265 3840x2160."
	echo "Type 1 for H265 1920x1080."
	echo "Type 2 for H265 1280x720."
	echo "Type 3 for H265 720x576."
	echo "Type 4 for H265 640x480."
	echo "Type 5 for H265 352x288."
	echo "Type 6 for H264 3840x2160."
	echo "Type 7 for H264 1920x1080."
	echo "Type 8 for H264 1280x720."
	echo "Type 9 for H264 720x576."
	echo "Type 10 for H264 640x480."
	echo "Type 11 for H264 352x288."
	echo "Type 12 for H265 2592x1944."
	echo "Type 13 for H264 2592x1944."
	echo "Type 14 for H265 2688x1520."
	echo "Type 15 for H264 2688x1520."
	echo "Type 'Enter' to ignore."
	read -p "Please select:" select
	case $select in
	0)
		start_rtsp $1 3840 2160 2
	;;
	1)
		start_rtsp $1 1920 1080 2
	;;
	2)
		start_rtsp $1 1280 720 2
	;;
	3)
		start_rtsp $1 720 576 2
	;;
	4)
		start_rtsp $1 640 480 2
	;;
	5)
		start_rtsp $1 352 288 2
	;;
	6)
		start_rtsp $1 3840 2160 1
	;;
	7)
		start_rtsp $1 1920 1080 1
	;;
	8)
		start_rtsp $1 1280 720 1
	;;
	9)
		start_rtsp $1 1280 720 1
	;;
	10)
		start_rtsp $1 640 480 1
	;;
	11)
		start_rtsp $1 352 288 1
	;;
	12)
		start_rtsp $1 2592 1944 2
	;;
	13)
		start_rtsp $1 2592 1944 1
	;;
	14)
		start_rtsp $1 2688 1520 2
	;;
	15)
		start_rtsp $1 2688 1520 1
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
	do_choice_port_on $1 $2
}
do_rtsp_dip_select() {
	echo "############################################"
	echo "Selected vpe--dip--venc--rtsp case"
	echo "Starting config vpe port$1 and venc output size."
	echo "Type 0 for H265 3840x2160."
	echo "Type 1 for H265 1920x1080."
	echo "Type 2 for H265 1280x720."
	echo "Type 3 for H265 720x576."
	echo "Type 4 for H265 640x480."
	echo "Type 5 for H265 352x288."
	echo "Type 6 for H264 3840x2160."
	echo "Type 7 for H264 1920x1080."
	echo "Type 8 for H264 1280x720."
	echo "Type 9 for H264 720x576."
	echo "Type 10 for H264 640x480."
	echo "Type 11 for H264 352x288."
	echo "Type 12 for H265 2592x1944."
	echo "Type 13 for H264 2592x1944."
	echo "Type 'Enter' to ignore."
	read -p "Please select:" select
	case $select in
	0)
		start_rtsp_with_dip $1 3840 2160 2
	;;
	1)
		start_rtsp_with_dip $1 1920 1080 2
	;;
	2)
		start_rtsp_with_dip $1 1280 720 2
	;;
	3)
		start_rtsp_with_dip $1 720 576 2
	;;
	4)
		start_rtsp_with_dip $1 640 480 2
	;;
	5)
		start_rtsp_with_dip $1 352 288 2
	;;
	6)
		start_rtsp_with_dip $1 3840 2160 1
	;;
	7)
		start_rtsp_with_dip $1 1920 1080 1
	;;
	8)
		start_rtsp_with_dip $1 1280 720 1
	;;
	9)
		start_rtsp_with_dip $1 720 576 1
	;;
	10)
		start_rtsp_with_dip $1 640 480 1
	;;
	11)
		start_rtsp_with_dip $1 352 288 1
	;;
	12)
		start_rtsp_with_dip $1 2592 1944 2
	;;
	13)
		start_rtsp_with_dip $1 2592 1944 1
	;;
	*)
		echo "Ignore !!!!"
		return
	;;
	esac
	do_choice_port_on $1 $2
}
do_jpeg_capture() {
	echo "############################################"
	echo "Which port do you want to do capture jpeg?"
	read -p "Please choose port:" port
	if [ ! "$port" ]; then
		return
	fi
	if [  $port -lt 0 -o $port -gt 3 ]; then
		echo "Error port number."
		return
	fi
	echo "############################################"
	echo "Selected vpe--venc capture jpeg case"
	echo "Starting config vpe port$1 and venc output size."
	echo "Type 0 for jpeg 3840x2160."
	echo "Type 1 for jpeg 1920x1080."
	echo "Type 2 for jpeg 1280x720."
	echo "Type 3 for jpeg 720x576."
	echo "Type 4 for jpeg 640x480."
	echo "Type 5 for jpeg 352x288."
	echo "Type 6 for jpeg 2592x1944."
	echo "Type 'Enter' to ignore."
	read -p "Please select:" select
	case $select in
	0)
		do_rtsp_capture_jpeg $port 3840 2160 
	;;
	1)
		do_rtsp_capture_jpeg $port 1920 1080
	;;
	2)
		do_rtsp_capture_jpeg $port 1280 720
	;;
	3)
		do_rtsp_capture_jpeg $port 720 576
	;;
	4)
		do_rtsp_capture_jpeg $port 640 480
	;;
	5)
		do_rtsp_capture_jpeg $port 352 288
	;;
	6)
		do_rtsp_capture_jpeg $port 2592 1944
	;;
	*)
		echo "Ignore !!!!"
	;;
	esac
}
do_baseinit() {
	./mdbenter integrate
	echo "############################################"
	echo "Selected vif--vpe case"
	echo "Starting config vpeRunMode."
	echo "Type 0 for Realtime."
	echo "Type 1 for RealTime Top."
	echo "Type 2 for Camera Frame."
	echo "Type 3 for Camera Frame Top."
	read -p "Please select:" RunMode
	case $RunMode in
	0)
		vpeworkmode=0x18
		vifworkmode=0
	;;
	1)
		vpeworkmode=0x08
		vifworkmode=0
	;;
	2)
		vpeworkmode=0x06
		vifworkmode=1
	;;
	3)
		vpeworkmode=0x02
		vifworkmode=1
	;;
	*)
		echo "Use default Realtime"
		vpeworkmode=0x18
		vifworkmode=0
	;;
	esac
	echo "Starting config vpeInput size."
	echo "Type 0 for 3840x2160."
	echo "Type 1 for 1920x1080."
	echo "Type 2 for 1280x720."
	echo "Type 3 for 720x576."
	echo "Type 4 for 640x480."
	echo "Type 5 for 352x288."
	echo "Type 6 for 2592x1944."
	echo "Type 7 for 2688x1520."
	echo "Type 'Enter' to ignore."
	read -p "Please select:" select
	case $select in
	0)
			vifwidth=3840
			vifheight=2160
	;;
	1)
			vifwidth=1920
			vifheight=1080
	;;
	2)
			vifwidth=1280
			vifheight=720
	;;
	3)
			vifwidth=720
			vifheight=576
	;;
	4)
			vifwidth=640
			vifheight=480
	;;
	5)
			vifwidth=352
			vifheight=288
	;;
	6)
			vifwidth=2592
			vifheight=1944
	;;
	7)
			vifwidth=2688
			vifheight=1520
	;;
	*)
		echo "Use default 1920x1080"
			vifwidth=1920
			vifheight=1080
	;;
	esac
	
	echo "############################################"
	echo "Selected vif framerate."
	read -p "Please set:" src_fps
	if [ "$src_fps" == "" ]; then
		src_fps="30"
		echo "Default use 30fps"
	fi
	echo "############################################"
	echo "Selected vpe framerate."
	read -p "Please set:" dest_fps
	if [ "$dest_fps" == "" ]; then
		dest_fps="30"
		echo "Default use 30fps"
	fi
	
		./mdbcmd vifinit $vifwidth $vifheight $vifworkmode
	
	if [ "$vpeworkmode" == "0x18" ]; then
		./mdbcmd vpeinit 0 $vifwidth $vifheight $vpeworkmode
		./mdbcmd iqopenserver $vifheight $vifheight 0
		./mdbcmd setbind 6 0 0 0 $src_fps 7 0 0 0 $dest_fps
		echo "vpe init realtime mode"
	fi
	
	if [ "$vpeworkmode" == "0x08" ]; then
		./mdbcmd vpeinit 1 $vifwidth $vifheight $vpeworkmode
		./mdbcmd vpecreateport 1 0 $vifwidth $vifheight 11
		./mdbcmd setbind 6 0 0 0 $src_fps 7 0 1 0 $dest_fps
		
		./mdbcmd iqopenserver $vifheight $vifheight 1	
		
		./mdbcmd vpeinit 0 $vifwidth $vifheight 0x10
		./mdbcmd setbind 7 0 1 0 $src_fps 7 0 0 0 $dest_fps
		echo "vpe init top-bottom mode"
	fi
	
	./mdbexit
}
do_basedeinit() {
	./mdbenter integrate
	if [ "$vpeworkmode" == "0x18" ]; then
		./mdbcmd setunbind 6 0 0 0 7 0 0 0
		./mdbcmd vpedeinit 0
	fi
	
	if [ "$vpeworkmode" == "0x08" ]; then
		./mdbcmd setunbind 7 0 1 0 7 0 0 0
		./mdbcmd vpedeinit 0
		./mdbcmd vpedestroyport 0 1
		./mdbcmd vpedeinit 1
		./mdbcmd setunbind 6 0 0 0 7 0 1 0
		
	fi
	./mdbcmd iqcloseserver
	./mdbcmd vifdeinit
	
	./mdbexit
}
do_something() {
	do_region_osd
	do_region_cover
	while true
	do
		for port in $port_array
		do
			echo "############################################"
			echo "Config port$port case"
			echo "Type 0 for vpe--venc--rtsp case"
			echo "Type 1 for vpe--dip--venc--rtsp case"
			read -p "Please select:" select
			case $select in
			0)
				do_rtsp_select $port $select
			;;
			1)
				do_rtsp_dip_select $port $select
			;;
			*)
				echo "Ignore !!!!"
			;;
			esac
		done
		echo "############################################"
		echo "Config panel case"
		echo "Type 0 to show vpe port0"
		echo "Type 1 to show vpe port1"
		echo "Type 2 to show vpe port2"
		echo "Type 3 to show vpe port3"
		read -p "Please select:" select
		do_panel_start $select
		b_ret=""
		while [ "$b_ret" != "r" -a "$b_ret" != "q" ]; do
			do_jpeg_capture
			echo "################################################"
			echo "Press 'r' to reset."
			echo "Press 'q' to exit."
			read -p "Press 'enter' to continue capture." b_ret
		done
		for port in $port_array
		do
			do_choice_port_off $port
		done
		if [ "$b_ret" == "q" ]; then
			return
		fi
	done
}
do_init
do_baseinit
do_something
do_basedeinit
do_deinit