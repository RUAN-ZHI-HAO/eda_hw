1. 學號：b11107011  
2. 姓名：阮致皓  
3. 使用之程式語言：C++  
4. 使用之編譯平台：Linux GNU g++  
5. 檔案壓縮方式: tar zcvf b11107011-p3.tgz b11107011-p3  
6. 各檔案說明：  
   b11107011-p3/main.cpp       : 主程式 source code   
   b11107011-p3/myStruct.h     : 資料結構定義 header file  
   b11107011-p3/myStruct.cpp   : 資料結構相關函式實作  
   b11107011-p3/function.h     : 主程式中的所有 function header file
   b11107011-p3/function.cpp   : 主程式中的所有 function 實作
   b11107011-p3/Makefile       : 編譯 設定檔  
   b11107011-p3/README.txt     : 本檔案  

7. 編譯方式說明：  
   在 b11107011-p3/ 這個資料夾下執行以下指令：make
   即可在 b11107011-p3 中產生 picRouting 的執行檔

8. 執行、使用方式說明：
   主程式：
   	   compile 完成後，在 b11107011-p3/ 目錄下會產生一個 picRouting 的執行檔
   	   執行檔的命令格式為 :
   	   [executable file name] [input file name] [output file name]

	   ex: ./picRouting pa3_resources/pic5x5.in pic5x5.out

    makefile：除了make可以使用還可以使用make out一次執行所有的東西，還有make check一次算所有的成績