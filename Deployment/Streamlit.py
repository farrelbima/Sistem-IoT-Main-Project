import streamlit as st
import mysql.connector
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

# Kredensial database MySQL 
db_config = {
    'user': 'root',
    'password': 'FarrelBimasena23',
    'host': '127.0.0.1',
    'database': 'testiot'
}

# Fungsi untuk mendapatkan data sensor terbaru dari MySQL
def get_sensor_data():
    # Koneksi ke database
    cnx = mysql.connector.connect(**db_config)
    cursor = cnx.cursor()

    # Query untuk mendapatkan satu data terakhir berdasarkan ID secara menurun
    query = ("SELECT * FROM testingiot ORDER BY id DESC LIMIT 1")

    cursor.execute(query)

    # Cek apakah ada data yang diambil
    data = cursor.fetchone()
    if data:
        # Membongkar empat nilai, mengabaikan id
        _, temperature, humidity, waterlevel = data

        # Cetak dan tampilkan data pada aplikasi Streamlit
        print(f"Temperature: {temperature} C")
        print(f"Humidity: {humidity} %")
        print(f"Water Level: {waterlevel}")

        st.write(f"**Latest Sensor Readings**")
        st.write(f"Temperature: {temperature} C")
        st.write(f"Humidity: {humidity} %")
        st.write(f"Water Level: {waterlevel}")
    else:
        # Menangani kasus tidak ada data (opsional)
        print("No data found in the database.")
        st.write("No recent sensor data available.")

    cursor.close()
    cnx.close()

# Fungsi untuk mendapatkan 5 data sensor terakhir dari MySQL
def get_last_five_records():
    # Koneksi ke database
    cnx = mysql.connector.connect(**db_config)
    cursor = cnx.cursor()

    # Query untuk mendapatkan 5 data terakhir berdasarkan ID secara menurun
    query = ("SELECT * FROM testingiot ORDER BY id DESC LIMIT 5")

    cursor.execute(query)

    # Mengambil semua baris
    rows = cursor.fetchall()

    cursor.close()
    cnx.close()

    # Mengubah menjadi DataFrame
    df = pd.DataFrame(rows, columns=['id', 'temperature', 'humidity', 'waterlevel'])
    
    # Membalik DataFrame untuk memiliki catatan paling lama terlebih dahulu
    df = df.iloc[::-1].reset_index(drop=True)
    
    return df

# Fungsi untuk membuat grafik garis menggunakan seaborn
def plot_line_graph():
    # Mengambil 5 catatan terakhir
    df = get_last_five_records()

    # Menyiapkan gambar matplotlib
    plt.figure(figsize=(10, 6))

    # Plot setiap kolom
    sns.lineplot(data=df, x=df.index, y='temperature', marker='o', label='Temperature')
    sns.lineplot(data=df, x=df.index, y='humidity', marker='o', label='Humidity')
    sns.lineplot(data=df, x=df.index, y='waterlevel', marker='o', label='Water Level')

    plt.xlabel('Record')
    plt.ylabel('Values')
    plt.title('Last 5 Sensor Readings')
    plt.legend()
    
    # Tampilkan plot pada Streamlit
    st.pyplot(plt)

# Tata letak aplikasi Streamlit
st.title('Sensor Data Dashboard')

# Tombol untuk mendapatkan data terbaru
if st.button('Get Latest Data'):
    get_sensor_data()

# Tombol untuk menampilkan grafik garis dari 5 catatan terakhir
if st.button('Show Last 5 Records Line Graph'):
    plot_line_graph()
