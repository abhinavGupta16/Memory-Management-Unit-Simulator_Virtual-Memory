mmu_abhinav_gupta: src/main.cpp
	g++ -g -std=c++11 src/Instruction.cpp src/VirtualMemoryAddress.cpp src/Process.cpp src/utils/Helper.cpp src/utils/ReadFile.cpp src/main.cpp -o mmu_abhinav_gupta
clean:
	rm -f mmu_abhinav_gupta *~
