# 🌐 CONFIGURACIÓN DE RED - SISTEMA MQTT

## Fecha: 18 de Octubre, 2025

---

## 📡 Información de Red WiFi

### Red WiFi Configurada:
```
SSID:     THETRUTH 4293
Password: J1234R000
Tipo:     2.4GHz (compatible con ESP32-S3)
```

### IP del Servidor Kali Linux:
```
IP:       192.168.137.17
Subnet:   192.168.137.0/24
```

---

## 🔧 Configuración del ESP32-S3

### WiFi (líneas 39-41 del código):
```cpp
const char* ssid = "THETRUTH 4293";
const char* password = "J1234R000";
```

### MQTT Broker (líneas 46-52 del código):
```cpp
const char* mqtt_server = "192.168.137.17";  // IP del Kali Linux
const int mqtt_port = 1883;
const char* mqtt_user = "";                   // Sin autenticación
const char* mqtt_password = "";
const char* device_id = "ESP32_01";
```

---

## ✅ Pasos para Conectar el ESP32-S3

### 1. **Verificar que el Kali Linux esté en la red**
```bash
# Verificar IP actual
ip addr show | grep "inet.*192.168.137"

# Debería mostrar: 192.168.137.17
```

### 2. **Verificar que MQTT esté escuchando en todas las interfaces**
```bash
# Ver puertos abiertos
sudo netstat -tuln | grep 1883

# Debería mostrar: 0.0.0.0:1883
```

### 3. **Cargar el código al ESP32-S3**
```
1. Abrir Arduino IDE
2. Archivo > Abrir > esp32_sensores.ino
3. Herramientas > Placa > ESP32S3 Dev Module
4. Herramientas > Puerto > Seleccionar puerto COM del ESP32
5. Subir (Upload)
```

### 4. **Monitorear conexión en Serial Monitor**
Abrir Monitor Serial (115200 baud), deberías ver:
```
============================================
ESP32 - SENSORES MQTT
Universidad Militar Nueva Granada
============================================

✅ Pines ADC configurados (8 potenciómetros)

Conectando a WiFi: THETRUTH 4293
......
✅ WiFi conectado
📡 Dirección IP: 192.168.137.XXX
📶 Intensidad señal: -XX dBm
Conectando a MQTT... ✅ Conectado
```

### 5. **Verificar mensajes MQTT en Kali Linux**
```bash
# Suscribirse a todos los tópicos
mosquitto_sub -h 192.168.137.17 -t "sensores/#" -v

# O usar localhost si estás en el mismo servidor
mosquitto_sub -h localhost -t "sensores/#" -v
```

---

## 🔍 Solución de Problemas

### ❌ ESP32 no se conecta a WiFi

**Causa posible:** Red 5GHz o SSID incorrecto  
**Solución:**
- Verificar que "THETRUTH 4293" sea red 2.4GHz
- Verificar que la contraseña sea exactamente: `J1234R000`
- Acercar el ESP32 al router

**Test en Kali Linux:**
```bash
# Ver redes disponibles
nmcli device wifi list | grep "THETRUTH"
```

### ❌ ESP32 conecta a WiFi pero no a MQTT

**Causa posible:** Firewall bloqueando puerto 1883  
**Solución:**
```bash
# Permitir puerto 1883 en firewall
sudo ufw allow 1883/tcp

# Verificar que Docker esté exponiendo el puerto
sudo docker ps | grep mqtt-server
# Debería mostrar: 0.0.0.0:1883->1883/tcp
```

### ❌ IP del Kali cambió

**Causa posible:** DHCP asignó nueva IP  
**Solución:**
```bash
# 1. Verificar nueva IP
ip addr show | grep "inet.*192.168"

# 2. Actualizar en esp32_sensores.ino (línea 46)
const char* mqtt_server = "NUEVA_IP_AQUI";

# 3. Recargar código al ESP32
```

**Para IP fija en Kali:**
```bash
# Editar conexión de red
sudo nmcli connection modify "THETRUTH 4293" ipv4.method manual
sudo nmcli connection modify "THETRUTH 4293" ipv4.addresses 192.168.137.17/24
sudo nmcli connection modify "THETRUTH 4293" ipv4.gateway 192.168.137.1
sudo nmcli connection modify "THETRUTH 4293" ipv4.dns "8.8.8.8 8.8.4.4"
sudo nmcli connection down "THETRUTH 4293"
sudo nmcli connection up "THETRUTH 4293"
```

### ❌ Mensajes no llegan a la base de datos

**Verificar pipeline completo:**
```bash
# 1. Verificar MQTT funciona
mosquitto_sub -h localhost -t "sensores/#" -v

# 2. Verificar suscriptor está corriendo
ps aux | grep suscriptor_admin

# 3. Si no está corriendo, iniciarlo
.venv/bin/python suscriptores/suscriptor_admin.py &

# 4. Verificar base de datos
.venv/bin/python consultar_db.py
```

---

## 📊 Topología de Red

```
Internet
   │
   ▼
Router WiFi "THETRUTH 4293" (2.4GHz)
   │
   ├─────────────────────┬──────────────────────┐
   │                     │                      │
   ▼                     ▼                      ▼
Kali Linux          ESP32-S3              Otros Dispositivos
192.168.137.17      192.168.137.XXX
   │
   ├─ Docker (MQTT Broker)
   │  └─ Puerto 1883
   │
   ├─ PostgreSQL
   │  └─ Base de datos mqtt_taller
   │
   └─ Python Scripts
      ├─ sensor_simulator.py
      ├─ suscriptor_admin.py
      └─ consultar_db.py
```

---

## 🔐 Seguridad

### Estado Actual:
- ✅ WiFi: Protegida con contraseña WPA2
- ⚠️ MQTT: Sin autenticación (`allow_anonymous = true`)
- ⚠️ PostgreSQL: Solo accesible desde localhost

### Mejoras Opcionales (Bonus +0.5 puntos):
```bash
# 1. Habilitar autenticación MQTT
mosquitto_passwd -c /etc/mosquitto/passwd mqtt_user

# 2. Configurar en mosquitto.conf:
allow_anonymous false
password_file /etc/mosquitto/passwd

# 3. Actualizar ESP32 con credenciales:
const char* mqtt_user = "mqtt_user";
const char* mqtt_password = "tu_password";
```

---

## 📝 Checklist de Conexión

Antes de presentar el proyecto, verificar:

- [ ] Kali Linux conectado a "THETRUTH 4293"
- [ ] IP del Kali es `192.168.137.17` (o actualizada en código)
- [ ] Docker MQTT corriendo: `sudo docker ps | grep mqtt-server`
- [ ] PostgreSQL activo: `sudo systemctl is-active postgresql`
- [ ] ESP32-S3 cargado con código actualizado
- [ ] 8 potenciómetros conectados a GPIO 1-8
- [ ] Monitor Serial muestra "WiFi conectado"
- [ ] Monitor Serial muestra "Conectado" a MQTT
- [ ] `mosquitto_sub` recibe mensajes del ESP32
- [ ] Base de datos recibe datos: `consultar_db.py`
- [ ] Suscriptor admin corriendo en background

---

## 📞 Comandos Rápidos de Verificación

```bash
# Todo en uno - Verificación completa
echo "=== Verificación del Sistema ===" && \
echo "1. Red WiFi:" && nmcli connection show | grep "THETRUTH" && \
echo "2. IP Kali:" && ip -4 addr show | grep "inet.*192.168" && \
echo "3. MQTT Docker:" && sudo docker ps --filter name=mqtt-server --format "{{.Status}}" && \
echo "4. PostgreSQL:" && sudo systemctl is-active postgresql && \
echo "5. Puerto MQTT:" && sudo netstat -tuln | grep 1883 && \
echo "=== Verificación Completa ==="
```

---

**Última actualización:** 18 de Octubre, 2025  
**Red configurada:** THETRUTH 4293  
**IP Servidor:** 192.168.137.17  
**Estado:** ✅ Configurado y listo para usar
