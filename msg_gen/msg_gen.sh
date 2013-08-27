#!/bin/bash

function get_msg_definition {
	IFS=$'\n'
	MSG_RAW=(`rosmsg show $1 -r`)

	unset MSG_DEF
	MSG_DEF=""
	for l in ${MSG_RAW[*]}; do

		if [[ $l != $'#'* ]] && [[ $l != $'\t'* ]] && [[ $l != $' '* ]] && [[ $l != [* ]]; then

			unset IFS
			words=($l)
			MSG_DEF="${MSG_DEF[@]} ${words[*]:0:2}"
			IFS=$'\n'
		fi
	done
	unset IFS

	MSG_DEF=($MSG_DEF)
}

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
c_types["string"]="char"*
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


msgs=("sensor_msgs/Image") #($@)


while((${#msgs[*]} > 0)); do
	m=${msgs[0]}
	MESSAGE_NAME_ROS=${m}
	MESSAGE_NAME=${MESSAGE_NAME_ROS/"/"/__}
	MESSAGE_NAME_UC=`echo ${MESSAGE_NAME} | tr [:lower:] [:upper:]`
	DEPENDANCY_INCLUDES=""

	get_msg_definition ${MESSAGE_NAME_ROS}
	STRUCT_FIELDS=""
	#echo "Message $m:   ${MSG_DEF[*]}"
	for ((i=0; i<${#MSG_DEF[*]}; i+=2)); do

		# Is the current field not a primitive type
		if [[ ${!c_types[*]} != *${MSG_DEF[$i]}* ]]; then
			# Add Header prefix separately
			if [[ ${MSG_DEF[$i]} == Header ]]; then
				MSG_DEF[$i]="std_msgs/Header"
			fi

			echo "Adding dependancy: ${MSG_DEF[$i]}"

			msgs=("${msgs[@]}" "${MSG_DEF[$i]}")
			c_types[${MSG_DEF[$i]}]=${MSG_DEF[$i]/"/"/__}
			DEPENDANCY_INCLUDES+="#include<${c_types[${MSG_DEF[$i]}]}.h>"$'\n'
		fi

		STRUCT_FIELDS+="    ${c_types[${MSG_DEF[$i]}]} ${MSG_DEF[$i + 1]};"$'\n'
	done
	STRUCT_FIELDS=${STRUCT_FIELDS%$'\n'} # Remove the last new line

	HEADER=`source ./msg_gen/h_template`

	echo "========================"
	msgs=(${msgs[@]:1})
done


