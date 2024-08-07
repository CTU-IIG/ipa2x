all:
	fastddsgen -replace -d ../warning_cpp/idl/ CarInfo.idl CrossingInfo.idl
	fastddsgen -replace -d ../IPA2XWarning/app/src/main/cpp/carinfo/ CarInfo.idl
	fastddsgen -replace -d ../IPA2XWarning/app/src/main/cpp/crossing/ CrossingInfo.idl
	fastddsgen -replace -d ../ipa2x_connect/idl/ CarInfo.idl CrossingInfo.idl

clean:
	rm -rf ../{warning_cpp/idl,IPA2XWarning/app/src/main/cpp/carinfo,IPA2XWarning/app/src/main/cpp/crossing,ipa2x_connect/idl}/*.{cxx,hpp,ipp,h}
