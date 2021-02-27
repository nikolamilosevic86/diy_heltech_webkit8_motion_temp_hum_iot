
# A very simple Flask Hello World app for you to get started with...

from flask import Flask
import MySQLdb as mdb
from flask import render_template
from flask import request

app = Flask(__name__)

@app.route('/anal')
def hello_world():
    username = 'XXXX'
    password = 'XXXX'
    hostname = 'XXXX.mysql.pythonanywhere-services.com'
    db = 'XXXX$smarthome'
    conn = mdb.connect(host=hostname, user=username, passwd=password, db=db)
    cursor = conn.cursor()
    content = '<h1>Nikola\'s home</h1><br/>'
    sql = 'select * from entries  order by id desc limit 100'
    cursor.execute(sql)
    rows = cursor.fetchall()
    for row in rows:
        content = content + 'time:'+str(row[1])+'; temperature: '+str(row[2])+' ; humidity: '+str(row[3])+'; motion: '+str(row[4])+'; ip: '+str(row[5])+'; <br/>'
    conn.close()
    return content

@app.route('/')
def analytics():
    username = 'XXXX'
    password = 'XXXX'
    hostname = 'XXXX.mysql.pythonanywhere-services.com'
    db = 'XXXX$smarthome'
    conn = mdb.connect(host=hostname, user=username, passwd=password, db=db)
    cursor = conn.cursor()
    sql = 'select * from entries  order by id desc limit 86400'
    cursor.execute(sql)
    rows = cursor.fetchall()
    times = []
    temperatures = []
    humidity = []
    movement = []
    for row in rows:

        times.append(row[1].strftime("%d/%m/%Y, %H:%M:%S"))
        temperatures.append(float(row[2][:-2]))
        humidity.append(float(row[3][:-1]))
        if row[4]=='':
            movement.append(0)
        else:
            movement.append(int(row[4])*100)
    conn.close()
    return render_template('chart_page.html',datetimes=times, temperature=temperatures,humidities = humidity,motion = movement)


@app.route('/add_data')
def add_data():
    username = 'XXXX'
    password = 'XXXX'
    hostname = 'XXXX.mysql.pythonanywhere-services.com'
    db = 'XXXX$smarthome'
    conn = mdb.connect(host=hostname, user=username, passwd=password, db=db)
    temperature = request.args.get('temp')
    humidity = request.args.get('humid')
    motion = request.args.get('motion')
    ip = request.args.get('ip')


    cursor = conn.cursor()
    sql = "INSERT INTO entries (temperature, humidity,movement,ip) VALUES (%s, %s,%s,%s)"
    val = (temperature, humidity,motion,ip)
    cursor.execute(sql, val)

    conn.commit()
    sql = "DELETE FROM entries WHERE id NOT IN (SELECT id FROM (SELECT id FROM entries ORDER BY id DESC LIMIT 180000) foo);"
    cursor.execute(sql)
    conn.commit()
    conn.close()
    return "Hello again"
