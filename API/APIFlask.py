import mysql.connector
from flask import Flask, request, jsonify

# Fungsi untuk menghubungkan ke database 
def connect_to_database():
    try:
        mydb = mysql.connector.connect(
            host='127.0.0.1',
            user='root',
            password='FarrelBimasena23',
            database='testiot'
        )
        return mydb
    except mysql.connector.Error as err:
        print("Error connecting to database:", err)
        return None  # Indikasi kegagalan koneksi

# Membuat aplikasi Flask
app = Flask(__name__)

# Endpoint API untuk menerima data
@app.route('/api/data', methods=['POST'])
def receive_data():
    # Menghubungkan ke database
    mydb = connect_to_database()
    if not mydb:
        return jsonify({'error': 'Failed to connect to database'}), 500

    # Mendapatkan data dari request (dalam format JSON)
    data = request.get_json()
    if not data:
        return jsonify({'error': 'Invalid JSON data'}), 400

    # Ekstraksi data dari JSON 
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    waterlevel = data.get('waterlevel')

    # Validasi tipe data 
    if not (isinstance(temperature, (int, float)) and isinstance(humidity, (int, float)) and isinstance(waterlevel, (int, float))):
        return jsonify({'error': 'Invalid data types. Temperature, humidity, and waterlevel must be numbers'}), 400

    # Pernyataan SQL 
    sql = "INSERT INTO testingiot (temperature, humidity, waterlevel) VALUES (%s, %s, %s)"
    values = (temperature, humidity, waterlevel)

    # Eksekusi query 
    try:
        mycursor = mydb.cursor()
        mycursor.execute(sql, values)
        mydb.commit()
        return jsonify({'message': 'Data received and stored successfully'}), 201
    except mysql.connector.Error as err:
        print("Error inserting data:", err)
        mydb.rollback()  # Rollback jika terjadi kesalahan
        return jsonify({'error': 'Failed to store data'}), 500
    finally:
        # Tutup cursor dan koneksi 
        if mycursor:
            mycursor.close()
        if mydb:
            mydb.close()

# Jalankan aplikasi Flask
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
