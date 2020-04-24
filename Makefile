mmu_abhinav_gupta: src/main.cpp
	g++ -g -std=c++11 src/utils/Helper.cpp src/pagers/Aging.cpp src/pagers/WorkingSet.cpp src/pagers/Random.cpp src/pagers/NotRecentlyUsed.cpp src/pagers/Clock.cpp src/utils/PageStats.cpp src/PageTableEntry.cpp src/FrameTableEntry.cpp src/VirtualMemoryAddress.cpp src/Process.cpp src/utils/ReadFile.cpp src/main.cpp src/pagers/FIFO.cpp -o mmu_abhinav_gupta; module load gcc-6.3.0;
clean:
	rm -f mmu_abhinav_gupta *~
