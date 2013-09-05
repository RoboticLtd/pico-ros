#!/bin/bash


declare -A c_types;
c_types["bool"]="uint8_t"
c_types["byte"]="int8_t"
c_types["char"]="uint8_t"
c_types["int8"]="int8_t"
c_types["uint8"]="uint8_t"
c_types["int16"]="int16_t"
c_types["uint16"]="uint16_t"
c_types["int32"]="int32_t"
c_types["uint32"]="uint32_t"
c_types["int64"]="int64_t"
c_types["uint64"]="uint64_t"
c_types["float32"]="float"
c_types["float64"]="double"
c_types["string"]="char*"
c_types["time"]="time"
c_types["duration"]="duration"
c_types["bool[]"]="uint8_t*"
c_types["byte[]"]="int8_t*"
c_types["char[]"]="uint8_t*"
c_types["int8[]"]="int8_t*"
c_types["uint8[]"]="uint8_t*"
c_types["int16[]"]="int16_t*"
c_types["uint16[]"]="uint16_t*"
c_types["int32[]"]="int32_t*"
c_types["uint32[]"]="uint32_t*"
c_types["int64[]"]="int64_t*"
c_types["uint64[]"]="uint64_t*"
c_types["float32[]"]="float*"
c_types["float64[]"]="double*"
c_types["string[]"]="char**"
c_types["time[]"]="time*"
c_types["duration[]"]="duration*"

declare -A msg_types;
for key in ${!c_types[*]}; do
	msg_types[${key}]=${c_types[${key}]}
done

msgs=("sensor_msgs/Image") #($@)

function get_msg_definition {
	IFS=$'\n'
	MSG_RAW=(`rosmsg show $1 -r`)

	unset MSG_DEF
	MSG_DEF=""
	for l in ${MSG_RAW[*]}; do

		if [[ $l =~ ^[a-zA-Z] ]]; then
			unset IFS
			words=($l)
			MSG_DEF="${MSG_DEF[@]} ${words[*]:0:2}"
			IFS=$'\n'
		fi
	done
	unset IFS

	MSG_DEF=($MSG_DEF)
}

function create_serialize {
	MSG_SER=""

	for w in ${struct_name_type}; do
		echo $w
	done
}



while((${#msgs[*]} > 0)); do
	m=${msgs[0]}
	MESSAGE_NAME_ROS=${m}
	MESSAGE_NAME=${MESSAGE_NAME_ROS/"/"/__}
	MESSAGE_NAME_UC=`echo ${MESSAGE_NAME} | tr [:lower:] [:upper:]`


	get_msg_definition ${MESSAGE_NAME_ROS}
	STRUCT_FIELDS=""
	DEPENDENCIES=""

	struct_name_type=""

	for ((i=0; i<${#MSG_DEF[*]}; i+=2)); do

		# Is the current field not a primitive type
		if [[ ${!c_types[*]} != *${MSG_DEF[$i]}* ]]; then
			# Add Header prefix separately
			if [[ ${MSG_DEF[$i]} == Header ]]; then
				MSG_DEF[$i]="std_msgs/Header"
			fi

			if [[ ${!msg_types[*]} != *${MSG_DEF[$i]}* ]]; then

				echo "Adding dependency: ${MSG_DEF[$i]}"
				msgs=("${msgs[@]}" "${MSG_DEF[$i]}")
				msg_types[${MSG_DEF[$i]}]=${MSG_DEF[$i]/"/"/__}

			fi

			DEPENDENCY_FLAG=`echo ${msg_types[${MSG_DEF[$i]}]} | tr [:lower:] [:upper:]`
			DEPENDENCIES+="#ifndef ${DEPENDENCY_FLAG}"$'\n'"#define ${DEPENDENCY_FLAG} 1"$'\n'"#endif"$'\n'
			DEPENDENCIES+="#include \"${msg_types[${MSG_DEF[$i]}]}.h\""$'\n\n'
		fi

		STRUCT_FIELDS+="    ${msg_types[${MSG_DEF[$i]}]} ${MSG_DEF[$i + 1]};"$'\n'
		struct_name_type+=" ${MSG_DEF[$i]} ${MSG_DEF[$i + 1]}"

		#echo ${struct_name_type[${MSG_DEF[$i + 1]}]}
	done
	STRUCT_FIELDS=${STRUCT_FIELDS%$'\n'} # Remove the last new line

	create_serialize

	#source ./msg_gen/h_template

	echo "========================"
	msgs=(${msgs[@]:1})
done


