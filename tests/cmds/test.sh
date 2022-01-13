touch tmp_x_file1
tmp_x_file1
echo $?
./tmp_x_file1
echo $?
export PATH=$HOME
echo $PATH
tmp_x_file1
echo $?
./tmp_x_file1
echo $?
unset PATH
tmp_x_file1
echo $?
./tmp_x_file1
echo $?
/bin/ls
/bin/rm -f tmp_x_file1