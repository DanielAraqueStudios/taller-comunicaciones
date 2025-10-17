# 📊 ESTADO DEL PROYECTO MQTT

**Fecha de actualización:** 2025-10-16  
**Universidad Militar Nueva Granada**  
**Taller Comunicaciones - 50% Segundo Corte**

---

## ✅ COMPLETADO (8/18 tareas - 44%)

### 📁 Estructura Base
- ✅ Carpetas creadas: `broker/`, `sensores/`, `suscriptores/`, `database/`, `docs/`
- ✅ Archivo README.md principal con toda la documentación

### 🔧 Broker MQTT
- ✅ `broker/docker-compose.yml` - Configuración Docker para Mosquitto
- ✅ `broker/mosquitto.conf` - Configuración completa del broker
- ✅ Soporte para instalación nativa en Kali Linux

### 🗄️ Base de Datos PostgreSQL
- ✅ `database/schema.sql` - Esquema completo con tablas, índices y vistas
- ✅ `database/db_config.py` - Módulo de conexión con funciones auxiliares
- ✅ Tabla `mensajes_mqtt` con todos los campos requeridos
- ✅ Vistas y funciones de estadísticas

### 📦 Dependencias
- ✅ `requirements.txt` - paho-mqtt, psycopg2-binary, python-dotenv
- ✅ `.env.example` - Plantilla de variables de entorno

### 📡 Publicadores (Sensores)
- ✅ `sensores/esp32_sensores.ino` - Código Arduino completo
  - DHT11 (Temperatura + Humedad)
  - MQ-2 (Sensor de humo)
  - LDR (Sensor de luz)
  - PIR (Sensor de movimiento)
  - Reed Switch (Sensor de puerta)
  - Botón de alarma manual
  - **Total: 7 sensores en 5 tópicos diferentes** ✅
  
- ✅ `sensores/sensor_simulator.py` - Simulador Python completo
  - Simula los 7 sensores
  - Publicación automática cada 5 segundos
  - Valores aleatorios realistas

### 🎧 Suscriptores
- ✅ `suscriptores/suscriptor_admin.py` - **Suscriptor administrativo completo**
  - Suscrito a todos los tópicos (#)
  - Almacena en PostgreSQL automáticamente
  - Manejo de errores y reconexión
  - Estadísticas en tiempo real

---

## 🚧 EN PROGRESO (1/18 tareas)

- 🔄 **Suscriptor 1 Temático (Bomberos)** - En desarrollo

---

## 📋 PENDIENTE (9/18 tareas - 50%)

### 🎧 Suscriptores Temáticos (3 restantes)
- ⏳ `suscriptor_2_tematico.py` - Vigilancia UMNG (seguridad + clima)
- ⏳ `suscriptor_3_tematico.py` - Profesor (iluminación + clima)
- ⏳ `suscriptor_4_tematico.py` - Policía (seguridad + incendio)

### 📚 Documentación (3 archivos)
- ⏳ `docs/INSTALACION.md` - Guía de instalación paso a paso
- ⏳ `docs/CONFIGURACION.md` - Configuración del sistema
- ⏳ `docs/PRUEBAS.md` - Casos de prueba y validación

### ⭐ Bonus (+0.5 puntos)
- ⏳ Autenticación MQTT (`broker/passwd`)
- ⏳ Conectividad desde Internet (port forwarding, DDNS)

---

## 📊 PROGRESO POR COMPONENTE

```
Estructura Base       ████████████████████ 100%
Broker MQTT          ████████████████████ 100%
Base de Datos        ████████████████████ 100%
Publicadores         ████████████████████ 100%
Suscriptor Admin     ████████████████████ 100%
Suscriptores Tema    █████░░░░░░░░░░░░░░░  25%
Documentación        ░░░░░░░░░░░░░░░░░░░░   0%
Bonus                ░░░░░░░░░░░░░░░░░░░░   0%
───────────────────────────────────────────
TOTAL                ████████░░░░░░░░░░░░  44%
```

---

## 📝 ARCHIVOS CREADOS

```
taller comunicaciones/
├── ✅ README.md (Documentación principal)
├── ✅ requirements.txt
├── ✅ .env.example
│
├── broker/
│   ├── ✅ docker-compose.yml
│   └── ✅ mosquitto.conf
│
├── sensores/
│   ├── ✅ esp32_sensores.ino (7 sensores)
│   └── ✅ sensor_simulator.py
│
├── suscriptores/
│   ├── ✅ suscriptor_admin.py (COMPLETO)
│   ├── ⏳ suscriptor_1_tematico.py (En progreso)
│   ├── ⏳ suscriptor_2_tematico.py (Pendiente)
│   ├── ⏳ suscriptor_3_tematico.py (Pendiente)
│   └── ⏳ suscriptor_4_tematico.py (Pendiente)
│
├── database/
│   ├── ✅ schema.sql
│   └── ✅ db_config.py
│
└── docs/
    ├── ⏳ INSTALACION.md (Pendiente)
    ├── ⏳ CONFIGURACION.md (Pendiente)
    └── ⏳ PRUEBAS.md (Pendiente)
```

---

## 🎯 PRÓXIMOS PASOS

### Prioridad Alta
1. **Completar los 4 suscriptores temáticos**
   - Implementar lógica específica para cada rol
   - Filtrado de tópicos correcto
   - Visualización en consola

2. **Crear documentación técnica**
   - INSTALACION.md con comandos para Kali Linux
   - CONFIGURACION.md con parámetros del sistema
   - PRUEBAS.md con casos de prueba

### Prioridad Media
3. **Pruebas de integración**
   - Instalar Mosquitto en Kali Linux
   - Configurar PostgreSQL
   - Ejecutar simulador y verificar base de datos

### Prioridad Baja (Bonus)
4. **Autenticación y acceso remoto**
   - Configurar usuarios con `mosquitto_passwd`
   - Port forwarding y DDNS
   - Documentar credenciales

---

## ✅ REQUISITOS DEL TALLER

| Requisito | Estado | Detalle |
|-----------|--------|---------|
| **5 Tópicos MQTT** | ✅ | incendio/, seguridad/, clima/, iluminacion/, sistema/ |
| **7+ Sensores** | ✅ | DHT11(2), MQ-2, LDR, PIR, Reed, Botón = 7 sensores |
| **2+ Sensores en ESP32** | ✅ | Todos los 7 sensores en un ESP32 |
| **4 Suscriptores Temáticos** | 🔄 | 1/4 completados (25%) |
| **1 Suscriptor Admin** | ✅ | Completo con almacenamiento en PostgreSQL |
| **Base de Datos** | ✅ | PostgreSQL con schema completo |
| **Timestamp recepción** | ✅ | Implementado en schema.sql |
| **Campo tópico** | ✅ | Implementado en schema.sql |

---

## 💡 NOTAS IMPORTANTES

### Para el ESP32
- Modificar `ssid` y `password` del WiFi
- Cambiar `mqtt_server` con la IP del servidor Kali Linux
- Instalar librerías: PubSubClient, DHT sensor library, ArduinoJson

### Para Kali Linux
```bash
# Instalar Mosquitto
sudo apt install mosquitto mosquitto-clients -y

# Instalar PostgreSQL
sudo apt install postgresql postgresql-contrib -y

# Instalar dependencias Python
pip install -r requirements.txt
```

### Para probar sin hardware
```bash
# Terminal 1: Iniciar simulador de sensores
python sensores/sensor_simulator.py

# Terminal 2: Iniciar suscriptor administrativo
python suscriptores/suscriptor_admin.py

# Terminal 3: Monitorear mensajes
mosquitto_sub -h localhost -t "#" -v
```

---

## 📞 CONTACTO

**Profesor:** Héctor Bernal  
**Correo:** hector.bernal@unimilitar.edu.co  
**Peso:** 50% del segundo corte

---

**Última actualización:** 2025-10-16 - 44% completado
