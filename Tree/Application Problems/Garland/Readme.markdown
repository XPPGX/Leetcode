# Garland

## problem link : https://codeforces.com/problemset/problem/767/C

## Run on dataset
```bash
#get answer
$./b <dataset_path> run
#example of getting answer
$./b dataset1.txt run

#check answer
$./b <dataset_path> <ans> <cut_node1> <cut_node2>
#example of checking answer
$./b dataset1.txt ans 3 5

#anwsers of datasets
---
# 0 <= node size <= 100
dataset 1 : 1, 4
dataset 2 : -1 (the total lights cannot be devided into 3 part)
dataset 3 : -1 (devided into 3 part, but no successful cut found)

# node size >= 100
dataset 4 : 63, 53
dataset 5 : 44, 49
dataset 6 : -1 (devided into 3 part, but no successful cut found)
dataset 7 : -1 (devided into 3 part, but no successful cut found)
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
