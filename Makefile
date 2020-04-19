mmu_abhinav_gupta: main.cpp
	g++ -g -std=c++11 Instruction.cpp VirtualMemoryAddress.cpp Process.cpp Helper.cpp main.cpp -o mmu_abhinav_gupta
clean:
	rm -f mmu_abhinav_gupta *~
