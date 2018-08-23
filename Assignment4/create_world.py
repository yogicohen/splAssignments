import sqlite3
import sys
import os.path
 
#if the db already exists the main func will do nothing
if (os.path.exists("world.db")==False):
    dbcon = sqlite3.connect('world.db')
    dbcon.executescript("""
        CREATE TABLE tasks (
            id      INTEGER         PRIMARY KEY AUTOINCREMENT,
            task_name    TEXT        NOT NULL,
            worker_id    INTEGER REFERENCES        workers(id),
            time_to_make    INTEGER        NOT NULL,
            resource_name    TEXT        NOT NULL,
            resource_amount    INTEGER        NOT NULL
        );
 
        CREATE TABLE workers (
            id                 INTEGER     PRIMARY KEY,
            name     TEXT    NOT NULL,
            status    TEXT        NOT NULL
        );
 
        CREATE TABLE resources (
            name    TEXT        PRIMARY KEY,
            amount    INTEGER        NOT NULL
            
        );
    """)
        
    my_lines = open(sys.argv[1])
    for line in my_lines:
        args = line.split(",")
            
        if(len(args)>3): #it's a task
            dbcon.execute("""
            INSERT INTO tasks (task_name, worker_id, time_to_make, resource_name, resource_amount) VALUES (?,?,?,?,?)
            """, [args[0],args[1],args[4],args[2],args[3]])
            continue
            
        if(len(args)>2): #it's a worker
            dbcon.execute("""
            INSERT INTO workers (id,name,status) VALUES (?,?,?)
            """, [args[1],args[2],'idle'])
            continue
            
            
        #it's a resource
        dbcon.execute("""
        INSERT INTO resources (name, amount) VALUES (?, ?)
        """, [args[0],args[1]])
    
    dbcon.commit()
