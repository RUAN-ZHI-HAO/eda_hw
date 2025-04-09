1. 學號：b11107011  
2. 姓名：阮致皓  
3. 使用之程式語言：C++  
4. 使用之編譯平台：Linux GNU g++  
5. 檔案壓縮方式: tar zcvf b11107011-p1.tgz b11107011-p1  
6. 各檔案說明：  
   b11107011-p1/main.cpp       : 主程式 source code  
   b11107011-p1/gnuPlot.h      : Gnuplot 相關函式 header file  
   b11107011-p1/gnuPlot.cpp    : Gnuplot 相關函式實作  
   b11107011-p1/myStruct.h     : 資料結構定義 header file  
   b11107011-p1/myStruct.cpp   : 資料結構相關函式實作  
   b11107011-p1/Makefile       : 編譯設定檔  
   b11107011-p1/readme.txt     : 本檔案  

7. 編譯方式說明：  
   在 b11107011-p1/ 這個資料夾下執行以下指令：make
   即可在 b11107011-p1 中產生 genPlot 的執行檔

8. 執行、使用方式說明：
   主程式：
   	   compile 完成後，在 b11107011-p1/ 目錄下會產生一個 genPlot 的執行檔
   	   執行檔的命令格式為 :
   	   ./genPlot [MSBCS width] [MSB CS height] [input file name] [output file name]

	   ex: ./genPlot 7100 6600 input.txt output.gp