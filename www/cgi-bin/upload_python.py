#! /usr/bin/python3
# -*- coding: utf-8 -*-
#
#   upload_python/upload_python.py
#
#                   Apr/11/2021
#
# --------------------------------------------------------------------
import os
import sys
import cgi
#
# --------------------------------------------------------------------
def single_upload_file_proc(upload_dir,item):
    filename = item.filename
    try:
        path = os.path.join(upload_dir,os.path.basename(filename))
        chunk = item.file.read()
        if chunk:
            fout = open(path,mode='wb')
            fout.write(chunk)
            print(filename + "<br />")
            fout.close()
            os.chmod(path, 0o666)
    except Exception as ee:
        print("*** error *** single_upload_file_proc ***<br />")
        print(filename + "<br />")
        print(str (ee))
        print("<br />")
# --------------------------------------------------------------------
def multi_uploaded_file_proc(upload_dir,fileitem):
#
    for item in fileitem:
        single_upload_file_proc(upload_dir,item)
#
# --------------------------------------------------------------------
upload_dir = "www/cgi-bin/upload"
#
print("Content-Type: text/html")
print("")

form = cgi.FieldStorage()
if "files" in form:
    fileitem = form["files"]
    print(isinstance (fileitem,list))
    print("<br />")
    if (isinstance(fileitem,list)):
        multi_uploaded_file_proc(upload_dir,fileitem)
    else:
        single_upload_file_proc(upload_dir,fileitem)
else:
    print("*** Select files ***<br />")

print("*** end ***<br />")
#
# --------------------------------------------------------------------
