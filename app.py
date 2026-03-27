import streamlit as st
import serial
import serial.tools.list_ports
import pandas as pd
import joblib
import time
import altair as alt

st.set_page_config(layout="wide", page_title="RF Intrusion Detector", page_icon="")

# --- UI Styling (Loaded once) ---
st.markdown("""
    <style>
    .status-normal { background-color: #064e3b; color: #34d399; padding: 15px; border-radius: 8px; text-align: center; font-size: 24px; font-weight: bold;}
    .status-alert { background-color: #7f1d1d; color: #f87171; padding: 15px; border-radius: 8px; text-align: center; font-size: 24px; font-weight: bold; animation: pulse 1s infinite;}
    @keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.6; } 100% { opacity: 1; } }
    </style>
""", unsafe_allow_html=True)

# --- State & AI Setup ---
if 'live_data' not in st.session_state:
    st.session_state.live_data = pd.DataFrame({'Channel': range(128), 'RSSI': [-105]*128, 'Color': ['#34D399']*128})

@st.cache_resource
def load_ai():
    try: return joblib.load("isolation_forest_model.pkl")
    except: return None
model = load_ai()

# --- Sidebar (Static) ---
with st.sidebar:
    st.title("Configure Ports")
    ports = [port.device for port in serial.tools.list_ports.comports()]
    selected_port = st.selectbox("Device Port:", ports if ports else ["None"])
    connect_btn = st.button("Connect")
    disconnect_btn = st.button("Disconnect")

# --- MAIN UI PLACEHOLDERS (These prevent blinking) ---
# We create these objects once. They stay on the screen permanently.
status_container = st.empty()
chart_container = st.empty()

# --- Connection Logic ---
if connect_btn and selected_port != "None":
    try:
        st.session_state.ser = serial.Serial(selected_port, 115200, timeout=0.1)
        st.session_state.is_connected = True
    except Exception as e:
        st.error(f"Error: {e}")

if disconnect_btn and 'ser' in st.session_state:
    st.session_state.ser.close()
    st.session_state.is_connected = False

# --- THE LIVE LOOP ---
if st.session_state.get('is_connected'):
    while True: # Keep the script alive here
        ser = st.session_state.ser
        if ser.in_waiting > 0:
            buffer_count = 0
            while ser.in_waiting > 0 and buffer_count < 300:
                try:
                    line = ser.readline().decode('utf-8').strip()
                    if ',' in line:
                        ch, rssi = map(int, line.split(','))
                        if 0 <= ch <= 127:
                            st.session_state.live_data.at[ch, 'RSSI'] = rssi
                            st.session_state.live_data.at[ch, 'Color'] = '#EF4444' if rssi > -50 else '#34D399'
                            
                            if ch == 127: # End of sweep logic
                                sweep = st.session_state.live_data['RSSI'].tolist()
                                is_anomaly = (model.predict([sweep])[0] == -1) if model else (max(sweep) > -40)
                                
                                # Update the BANNER without refreshing the page
                                if is_anomaly:
                                    status_container.markdown('<div class="status-alert">🚨 INTRUSION DETECTED 🚨</div>', unsafe_allow_html=True)
                                    ser.write(b'A')
                                else:
                                    status_container.markdown('<div class="status-normal">🟢 SPECTRUM SECURE</div>', unsafe_allow_html=True)
                                    ser.write(b'N')
                    buffer_count += 1
                except: pass

            # Update the CHART without refreshing the page
            chart = alt.Chart(st.session_state.live_data).mark_bar(size=5).encode(
                x=alt.X('Channel:Q', scale=alt.Scale(domain=[0, 127])),
                y=alt.Y('RSSI:Q', scale=alt.Scale(domain=[-105, -20])),
                color=alt.Color('Color:N', scale=None)
            ).properties(height=400, width='container')
            
            chart_container.altair_chart(chart, use_container_width=True)
        
        time.sleep(0.01) # Small sleep to prevent CPU 100% usage
else:
    status_container.info("Connect to the ESP32 to begin scan.")