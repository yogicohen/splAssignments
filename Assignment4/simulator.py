import sqlite3
import sys
import os.path

#object to represent task - to make life (and access) easier
class Task(object):
    def __init__(self, id, task_name, worker_id, time_to_make, resource_name, resource_amount):
        self.id = id
        self.task_name = task_name
        self.worker_id = worker_id
        self.time_to_make = time_to_make
        self.resource_name = resource_name
        self.resource_amount = resource_amount

#returns all tasks available
def getTasks(dbcon):
    c = dbcon.cursor()
    allTasks = c.execute("""
            SELECT id,task_name, worker_id, time_to_make, resource_name, resource_amount FROM tasks
                         """).fetchall()
    return [Task(*row) for row in allTasks]


if (os.path.exists("world.db") == True):
    dbcon = sqlite3.connect('world.db')
    tasksToDo = getTasks(dbcon)
    #dict object to hold slave id vs the task he's currently busy on
    slaveIdVsTaskName = {}
    while ((len(tasksToDo) > 0) and (os.path.exists("world.db"))):
        for task in tasksToDo:
            workerIdToEnslave = task.worker_id
            workerNameAndStatus = dbcon.cursor().execute("""
                            SELECT name,status FROM workers WHERE id=(?)
                            """, [task.worker_id]).fetchone()
            if (workerNameAndStatus[1] == 'idle'):
                dbcon.execute("UPDATE workers SET status=(?) WHERE id=(?)", ('busy', workerIdToEnslave))
                resourceAmount = dbcon.cursor().execute("SELECT amount FROM resources WHERE name=(?)"
                                                        , [task.resource_name]).fetchone()
                amountToUpdate = resourceAmount[0] - task.resource_amount
                dbcon.execute("UPDATE resources SET amount=(?) WHERE name=(?)", (task.resource_name, amountToUpdate))
                slaveIdVsTaskName.update({workerIdToEnslave: task.task_name})
                dbcon.commit()
                print('{} says: work work'.format(workerNameAndStatus[0].strip()))

            if (workerNameAndStatus[1] == 'busy' and slaveIdVsTaskName.get(task.worker_id)==task.task_name):
                print("{} is busy {}...".format((workerNameAndStatus[0].strip()), task.task_name))
                Timetomake = dbcon.cursor().execute("SELECT time_to_make FROM tasks WHERE task_name=(?)",
                                                    [task.task_name]).fetchone()
                timeToUpdate = Timetomake[0] - 1
                dbcon.execute("UPDATE tasks SET time_to_make=(?) WHERE task_name=(?)", (timeToUpdate, task.task_name))
                dbcon.commit()

        #iterating again over the tasks to see which has been done
        for task in tasksToDo:
            time = dbcon.cursor().execute("SELECT time_to_make FROM tasks WHERE task_name=(?)",
                                          [task.task_name]).fetchone()
            if time[0] == 0:
                dbcon.execute("DELETE FROM tasks WHERE task_name=(?)", [task.task_name])
                dbcon.execute("UPDATE workers SET status=(?) WHERE id=(?)", ('idle', task.worker_id))
                name = dbcon.cursor().execute("SELECT name FROM workers WHERE id=(?)",
                                              [task.worker_id]).fetchone()
                dbcon.commit()
                print('{} says: All Done!'.format(name[0].strip()))

        #refreshing tasksToDo cause maybe some has been deleted
        tasksToDo = getTasks(dbcon)