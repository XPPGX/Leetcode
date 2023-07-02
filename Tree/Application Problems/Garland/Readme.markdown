# Garland

## problem link : https://codeforces.com/problemset/problem/767/C

## Run on dataset
```bash
#get answer
$./a <dataset_path>
#example of get answer
$./a dataset1.txt

#anwsers of datasets
dataset 1 : 1, 4
dataset 2 : -1 (the total lights cannot be devided into 3 part)
dataset 3 : -1 (devided into 3 part, but no successful cut found)
```

## Create Dataset
```bash
#create fail dataset
$ ./DataGen fail <node_num> <total_light> <max_single_light> <parent or leaf> <dataset_path>
#example of create fail dataset
$ ./DataGen fail 12 99 15 parent Fail_dataset_1.txt


#create success dataset
$ ./DataGen success <node_num> <total_light> <max_single_light> <pp or pl or ll> <dataset_path>
#example of create success dataset
$ ./DataGen success 50 144 15 pl Succ_dataset_1.txt
```
