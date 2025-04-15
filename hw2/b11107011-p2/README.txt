1. 學號：b11107011  
2. 姓名：阮致皓  
3. 使用之程式語言：C++  
4. 使用之編譯平台：Linux GNU g++  
5. 檔案壓縮方式: tar zcvf b11107011-p2.tgz b11107011-p2  
6. 各檔案說明：  
   b11107011-p2/main.cpp       : 主程式 source code   
   b11107011-p2/myStruct.h     : 資料結構定義 header file  
   b11107011-p2/myStruct.cpp   : 資料結構相關函式實作  
   b11107011-p2/function.h     : 主程式中的所有 function header file
   b11107011-p2/function.cpp   : 主程式中的所有 function 實作
   b11107011-p2/Makefile       : 編譯 設定檔  
   b11107011-p2/readme.txt     : 本檔案  

7. 編譯方式說明：  
   在 b11107011-p2/ 這個資料夾下執行以下指令：make
   即可在 b11107011-p2 中產生 legalizer 的執行檔

8. 執行、使用方式說明：
   主程式：
   	   compile 完成後，在 b11107011-p2/ 目錄下會產生一個 legalizer 的執行檔
   	   執行檔的命令格式為 :
   	   ./legalizer [cell width (in terms of sites)] [alpha] [input file name] [output file name]

	   ex: ./legalizer 1 0.5 pa2_testcases/case1.def case1

   畫圖：如果需要畫出布局的樣子可以去取消註解相關的程式碼

   限制：在super1中不可到5倍大，super2中不可到4倍大