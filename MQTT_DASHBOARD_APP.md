# 📱 CONFIGURACIÓN MQTT DASHBOARD APP

## Conexión al Servidor MQTT desde Móvil

---

## 📋 DATOS DE CONEXIÓN

### **Configuración para MQTT Dashboard (Android/iOS):**

```
Client ID:       MQTT_Dashboard_Mobile_01
                (o cualquier nombre único que desees)

Server Name:     192.168.137.17
                (IP del servidor Kali Linux)

Port:            1883
                (Puerto MQTT estándar - sin SSL)

Username:        (dejar en blanco)
                (sin autenticación configurada)

Password:        (dejar en blanco)
                (sin autenticación configurada)

Keep Alive:      60
                (segundos)

Clean Session:   ✓ Enabled
                (activar)

Protocol:        TCP
                (no usar SSL/TLS)
```

---

## 🔧 CONFIGURACIÓN PASO A PASO

### **1. Abrir MQTT Dashboard App**

### **2. Crear Nueva Conexión:**
- Tap en **"+"** o **"Add Connection"**

### **3. Llenar los campos:**

#### **General Tab:**
```
Connection Name:  Universidad Militar (o el nombre que prefieras)
Client ID:        MQTT_Dashboard_Mobile_01
Server:           192.168.137.17
Port:             1883
```

#### **Authentication (dejar vacío):**
```
Username:         (vacío)
Password:         (vacío)
```

#### **Advanced Settings:**
```
Keep Alive:       60
Clean Session:    ✓ ON
Auto Reconnect:   ✓ ON
Protocol:         MQTT 3.1.1
```

### **4. Guardar y Conectar:**
- Tap en **"Save"**
- Tap en **"Connect"**
- Deberías ver: **"Connected"** ✅

---

## 📊 TÓPICOS PARA SUSCRIBIRSE

### **Opción 1 - Suscribirse a TODOS los sensores:**
```
Topic: sensores/#
QoS:   0 o 1
```

### **Opción 2 - Suscribirse por categoría:**

#### **Clima:**
```
clima/temperatura
clima/humedad
clima/viento
```

#### **Incendio:**
```
incendio/sensor_humo
incendio/alarma
```

#### **Seguridad:**
```
seguridad/puerta
seguridad/movimiento
```

#### **Iluminación:**
```
iluminacion/luz
```

#### **Sistema:**
```
sistema/estado
```

---

## 🎨 WIDGETS RECOMENDADOS EN MQTT DASHBOARD

### **1. Para Temperatura (clima/temperatura):**
```
Widget Type:    Gauge (medidor circular)
Topic:          clima/temperatura
Min Value:      0
Max Value:      50
Units:          °C
JSON Path:      $.value
```

### **2. Para Humedad (clima/humedad):**
```
Widget Type:    Progress Bar
Topic:          clima/humedad
Min Value:      0
Max Value:      100
Units:          %
JSON Path:      $.value
```

### **3. Para Humo (incendio/sensor_humo):**
```
Widget Type:    Gauge
Topic:          incendio/sensor_humo
Min Value:      0
Max Value:      100
Units:          %
JSON Path:      $.value
Color:          Red (si > 50%)
```

### **4. Para Puerta (seguridad/puerta):**
```
Widget Type:    LED Indicator
Topic:          seguridad/puerta
ON Value:       1
OFF Value:      0
JSON Path:      $.value
Labels:         "Abierta" / "Cerrada"
```

### **5. Para Movimiento (seguridad/movimiento):**
```
Widget Type:    LED Indicator
Topic:          seguridad/movimiento
ON Value:       1
OFF Value:      0
JSON Path:      $.value
Labels:         "Detectado" / "Sin movimiento"
```

### **6. Para Luz (iluminacion/luz):**
```
Widget Type:    Progress Bar
Topic:          iluminacion/luz
Min Value:      0
Max Value:      100
Units:          %
JSON Path:      $.value
```

### **7. Para Viento (clima/viento):**
```
Widget Type:    Gauge
Topic:          clima/viento
Min Value:      0
Max Value:      100
Units:          km/h
JSON Path:      $.value
```

---

## 📝 FORMATO DE MENSAJES JSON

Los mensajes que recibirás tienen este formato:

```json
{
  "device_id": "ESP32_01",
  "value": 25.5,
  "unit": "°C",
  "status": "normal",
  "timestamp": 12345
}
```

**Para extraer valores en MQTT Dashboard:**
- **Valor numérico:** Usar JSON Path: `$.value`
- **Unidad:** Usar JSON Path: `$.unit`
- **Estado:** Usar JSON Path: `$.status`
- **Device ID:** Usar JSON Path: `$.device_id`

---

## 🔍 SOLUCIÓN DE PROBLEMAS

### ❌ "Connection Failed" o "Connection Timeout"

**Causa 1:** El móvil no está en la misma red WiFi
```
Solución: Conectar el móvil a la red "THETRUTH 4293"
```

**Causa 2:** IP incorrecta
```
Solución: Verificar la IP del servidor Kali:
En Kali: ip addr show | grep "inet 192.168"
Usar esa IP en la app
```

**Causa 3:** Puerto MQTT bloqueado
```
Solución: Verificar que el puerto 1883 esté abierto:
En Kali: sudo netstat -tuln | grep 1883
```

### ❌ "Connected" pero no recibo mensajes

**Causa:** No hay sensores publicando o suscripción incorrecta
```
Solución 1: Verificar que el simulador esté corriendo:
.venv/bin/python sensores/sensor_simulator.py

Solución 2: Suscribirse al topic correcto:
Usar: sensores/# (con el wildcard #)
```

### ❌ Mensajes llegan pero no se muestran en los widgets

**Causa:** JSON Path incorrecto
```
Solución: Usar $.value para extraer el valor numérico
```

---

## 🎯 EJEMPLO COMPLETO DE DASHBOARD

### **Layout Recomendado:**

```
┌─────────────────────────────────────┐
│         🌡️ CLIMA                    │
├─────────────────────────────────────┤
│ Temperatura: [Gauge 0-50°C]        │
│ Humedad:     [Progress 0-100%]     │
│ Viento:      [Gauge 0-100 km/h]    │
├─────────────────────────────────────┤
│         🔥 INCENDIO                 │
├─────────────────────────────────────┤
│ Humo:        [Gauge 0-100%]        │
│ Alarma:      [LED Indicator]       │
├─────────────────────────────────────┤
│         🔒 SEGURIDAD                │
├─────────────────────────────────────┤
│ Puerta:      [LED: Abierta/Cerrada]│
│ Movimiento:  [LED: Sí/No]          │
├─────────────────────────────────────┤
│         💡 ILUMINACIÓN              │
├─────────────────────────────────────┤
│ Luz Solar:   [Progress 0-100%]     │
└─────────────────────────────────────┘
```

---

## 📱 APPS RECOMENDADAS

### **Android:**
1. **MQTT Dashboard (IoT)** by Ravendran Softwares
   - ⭐ Más popular
   - Widgets variados
   - JSON Path support

2. **IoT MQTT Panel** by MQTT Solutions
   - Interface moderna
   - Fácil configuración

3. **Linear MQTT Dashboard** by Cygnux
   - Gratuita
   - Simple y funcional

### **iOS:**
1. **MQTTool** by Torsten Teuber
   - Profesional
   - JSON support

2. **MQTT Explorer** by MQTT-Explorer
   - Visualización de tópicos
   - Debug avanzado

---

## 🔐 SEGURIDAD (Opcional - Bonus)

### **Para habilitar autenticación en el futuro:**

**En el servidor Kali:**
```bash
# Crear usuario MQTT
sudo docker exec -it mqtt-server mosquitto_passwd -c /mosquitto/config/passwd mqtt_user

# Editar mosquitto.conf
# Cambiar: allow_anonymous false
# Agregar: password_file /mosquitto/config/passwd
```

**En MQTT Dashboard App:**
```
Username: mqtt_user
Password: [la contraseña que configuraste]
```

---

## ✅ CHECKLIST DE CONEXIÓN

Antes de conectarte desde la app, verificar:

- [ ] Móvil conectado a "THETRUTH 4293"
- [ ] Servidor Kali encendido y en la misma red
- [ ] MQTT broker corriendo: `sudo docker ps | grep mqtt-server`
- [ ] IP del servidor: `192.168.137.17` (verificar con `ip addr`)
- [ ] Puerto 1883 abierto
- [ ] Sensores publicando datos (simulador o ESP32)

---

## 📞 COMANDOS DE VERIFICACIÓN

### **En el servidor Kali:**

```bash
# 1. Verificar IP
ip addr show | grep "inet 192.168"

# 2. Verificar MQTT está corriendo
sudo docker ps | grep mqtt-server

# 3. Ver mensajes MQTT en tiempo real
mosquitto_sub -h localhost -t "sensores/#" -v

# 4. Iniciar simulador (si no está corriendo)
.venv/bin/python sensores/sensor_simulator.py
```

### **Probar desde el móvil (con app MQTT Dashboard):**
1. Conectar a 192.168.137.17:1883
2. Suscribirse a `sensores/#`
3. Deberías ver mensajes cada 5 segundos

---

## 🎓 TIPS ADICIONALES

1. **Client ID único:** Si conectas múltiples dispositivos, usa IDs diferentes:
   - Móvil 1: `MQTT_Dashboard_Mobile_01`
   - Móvil 2: `MQTT_Dashboard_Mobile_02`
   - Tablet: `MQTT_Dashboard_Tablet_01`

2. **QoS (Quality of Service):**
   - **QoS 0:** Máximo rendimiento (recomendado para sensores)
   - **QoS 1:** Garantiza entrega (usa si es crítico)
   - **QoS 2:** Entrega exactamente una vez (más lento)

3. **Colores en Widgets:**
   - Verde: Valores normales
   - Amarillo: Advertencia (50-80%)
   - Rojo: Alerta (>80%)

4. **Retain Message:**
   - Habilitar para ver el último valor al conectar
   - Útil para sensores que no publican frecuentemente

---

## 📊 RESUMEN RÁPIDO

```
═══════════════════════════════════════════════════════
          CONFIGURACIÓN MQTT DASHBOARD APP
═══════════════════════════════════════════════════════

Server:           192.168.137.17
Port:             1883
Client ID:        MQTT_Dashboard_Mobile_01
Username:         (vacío)
Password:         (vacío)
Topics:           sensores/#

Red WiFi:         THETRUTH 4293

═══════════════════════════════════════════════════════
```

---

**Última actualización:** 18 de Octubre, 2025  
**Estado:** ✅ Sin autenticación - Configuración simple  
**Compatible con:** MQTT Dashboard (Android/iOS), IoT MQTT Panel, MQTTool
