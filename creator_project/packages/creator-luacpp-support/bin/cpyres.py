#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import os, sys, shutil, hashlib, glob

def file_hash(filename, hash_type='md5'):  
    hash_func = hashlib.new(hash_type)  
    with open(filename, 'rb') as file:  
        for chunk in iter(lambda: file.read(4096), b''):  
            hash_func.update(chunk)  
    return hash_func.hexdigest()  

def are_files_equal_by_hash(file1, file2, hash_type='md5'):  
    return file_hash(file1, hash_type) == file_hash(file2, hash_type) 

def are_files_new_by_mtime(fromFile, toFile):
    return os.path.getmtime(fromFile) > os.path.getmtime(toFile)
	
def copyNewestFiles(src, dst, suffix):
	# 使用glob模块搜索所有后缀 suffix 文件  
	files = glob.glob(os.path.join(src, '**', suffix), recursive=True)
	for f in files:
		toFile = f.replace(src, dst)
		if (not os.path.isfile(toFile)) or (are_files_new_by_mtime(f, toFile)):
			# 使用shutil模块中的copy2函数来复制文件 :会尝试复制文件的元数据（如时间戳）
			shutil.copy2(f, toFile)
			# 使用字符串格式化来拼接参数
			# command = 'cp -rfp'
			# full_command = f'{command} "{f}" "{toFile}"' 
			# os.system(full_command)
		pass

if __name__ == "__main__":
	# dir = sys.argv[0]
	pathSrc = sys.argv[1]
	pathDst = sys.argv[2]
	print(pathSrc, pathDst)
	ccreator = os.path.join(pathSrc, 'ccreator')
	assets = os.path.join(pathSrc, 'assets')
	copyNewestFiles(ccreator,pathDst, '*.ccreator')
	copyNewestFiles(assets,  pathDst, '*.png')
	copyNewestFiles(assets,  pathDst, '*.jpg')
	copyNewestFiles(assets,  pathDst, '*.plist')
	copyNewestFiles(assets,  pathDst, '*.json')
	copyNewestFiles(assets,  pathDst, '*.atlas')
	copyNewestFiles(assets,  pathDst, '*.fnt')

