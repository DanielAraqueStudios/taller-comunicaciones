# ✅ CONFIGURACIÓN COMPLETADA - PostgreSQL Remoto

## Fecha: 18 de Octubre, 2025

---

## 🎉 ESTADO DEL SISTEMA

### ✅ Servicios Operativos:
- **PostgreSQL:** Activo y escuchando en `0.0.0.0:5432`
- **MQTT Broker:** Activo (Docker container "mqtt-server")
- **Base de datos:** mqtt_taller con 56 mensajes
- **Conexiones remotas:** ✅ Habilitadas y probadas

---

## 📋 CONFIGURACIÓN PARA EL OTRO COMPUTADOR

### **Archivo .env que debe usar:**

```env
# Copiar este contenido al archivo .env del otro computador
DB_HOST=192.168.137.17
DB_PORT=5432
DB_NAME=mqtt_taller
DB_USER=mqtt_admin
DB_PASSWORD=mqtt_secure_2025

MQTT_BROKER=192.168.137.17
MQTT_PORT=1883
DEVICE_ID=REMOTE_01
```

---

## 🧪 PRUEBAS DE CONEXIÓN

### **1. Probar conexión PostgreSQL desde el otro computador:**

#### Opción A - Usando psql (si está instalado):
```bash
PGPASSWORD=mqtt_secure_2025 psql -h 192.168.137.17 -U mqtt_admin -d mqtt_taller
```

#### Opción B - Usando Python:
```python
import psycopg2

try:
    conn = psycopg2.connect(
        host="192.168.137.17",
        port=5432,
        database="mqtt_taller",
        user="mqtt_admin",
        password="mqtt_secure_2025"
    )
    print("✅ Conexión exitosa a PostgreSQL")
    cursor = conn.cursor()
    cursor.execute("SELECT COUNT(*) FROM mensajes_mqtt;")
    count = cursor.fetchone()[0]
    print(f"📊 Mensajes en la base de datos: {count}")
    cursor.close()
    conn.close()
except Exception as e:
    print(f"❌ Error: {e}")
```

### **2. Probar MQTT desde el otro computador:**
```bash
mosquitto_sub -h 192.168.137.17 -t "sensores/#" -v
```

---

## 🔧 CONFIGURACIÓN REALIZADA EN EL SERVIDOR

### ✅ Cambios en PostgreSQL:

1. **postgresql.conf:**
   - `listen_addresses = '*'` (escucha en todas las interfaces)

2. **pg_hba.conf:**
   - Regla agregada: `host mqtt_taller mqtt_admin 192.168.137.0/24 md5`
   - Permite conexiones desde toda la red 192.168.137.0/24

3. **Usuario mqtt_admin:**
   - Contraseña reconfigurada: `mqtt_secure_2025`
   - Privilegios completos en la base de datos mqtt_taller

4. **Servicio:**
   - PostgreSQL reiniciado y verificado
   - Estado: ✅ Active

---

## 📊 VERIFICACIÓN DEL SISTEMA

### Estado actual confirmado:
```
✅ PostgreSQL: active
✅ Escuchando en: 0.0.0.0:5432 (todas las interfaces)
✅ Conexión remota: Funcional (probada con IP 192.168.137.17)
✅ Base de datos: 56 mensajes almacenados
✅ MQTT Docker: Up 6 hours
```

---

## 🚨 SOLUCIÓN DE PROBLEMAS

### ❌ "Connection refused" desde el otro computador

**Posibles causas:**
1. Firewall bloqueando el puerto 5432
2. Ambos computadores no están en la misma red

**Soluciones:**
```bash
# En el servidor Kali, verificar que el puerto esté abierto:
sudo netstat -tuln | grep 5432

# Verificar conectividad de red:
ping 192.168.137.17

# Verificar que ambos estén en la misma red WiFi (THETRUTH 4293)
```

### ❌ "Password authentication failed"

**Causa:** Contraseña incorrecta

**Solución:**
```bash
# En el servidor Kali, resetear contraseña:
sudo -u postgres psql -c "ALTER USER mqtt_admin WITH PASSWORD 'mqtt_secure_2025';"
```

### ❌ "No route to host"

**Causa:** Firewall o problema de red

**Solución en el servidor Kali:**
```bash
# Verificar IP actual:
ip addr show | grep "inet 192.168"

# Si la IP cambió, actualizar en el .env del otro computador
```

---

## 📁 ARCHIVOS DE REFERENCIA

### En el servidor Kali:
- **Configuración:** `.env` (usa `DB_HOST=localhost`)
- **Scripts:**
  - `setup_sistema.py` - Configuración automática completa
  - `configurar_postgresql_remoto.py` - Configuración de acceso remoto
  - `consultar_db.py` - Consultar datos de la base de datos

### En el otro computador:
- **Configuración:** `.env` (debe usar `DB_HOST=192.168.137.17`)
- **Archivo de ejemplo:** `.env.remoto` (en el servidor, copiar al otro computador)

---

## 🔐 SEGURIDAD

### Estado Actual:
- ✅ PostgreSQL: Autenticación con contraseña (md5)
- ✅ Acceso limitado a red local (192.168.137.0/24)
- ⚠️ MQTT: Sin autenticación (allow_anonymous = true)

### Mejoras Opcionales:
1. **Autenticación MQTT:** Configurar usuario/contraseña en Mosquitto
2. **SSL/TLS:** Encriptar conexiones PostgreSQL y MQTT
3. **VPN:** Para acceso seguro desde fuera de la red local

---

## 📞 COMANDOS ÚTILES

### En el servidor Kali:
```bash
# Ver estado de PostgreSQL
sudo systemctl status postgresql

# Ver conexiones activas
sudo -u postgres psql -c "SELECT client_addr, usename, application_name FROM pg_stat_activity WHERE client_addr IS NOT NULL;"

# Ver logs de PostgreSQL
sudo tail -f /var/log/postgresql/postgresql-*-main.log

# Reiniciar PostgreSQL
sudo systemctl restart postgresql

# Ver datos en la base
.venv/bin/python consultar_db.py
```

### En el otro computador:
```bash
# Probar conexión
PGPASSWORD=mqtt_secure_2025 psql -h 192.168.137.17 -U mqtt_admin -d mqtt_taller -c "SELECT COUNT(*) FROM mensajes_mqtt;"

# Ejecutar suscriptor (si tienes el código)
python suscriptores/suscriptor_admin.py

# Ver mensajes MQTT
mosquitto_sub -h 192.168.137.17 -t "sensores/#" -v
```

---

## ✅ CHECKLIST FINAL

Antes de usar desde el otro computador, verificar:

- [x] Servidor PostgreSQL activo en Kali
- [x] Escuchando en 0.0.0.0:5432
- [x] Usuario mqtt_admin configurado con contraseña correcta
- [x] Regla de acceso en pg_hba.conf agregada
- [x] Conexión probada exitosamente
- [ ] Archivo .env configurado en el otro computador con IP: 192.168.137.17
- [ ] Ambos computadores en la misma red WiFi (THETRUTH 4293)
- [ ] Dependencias Python instaladas en el otro computador (psycopg2-binary)

---

## 🎯 PRÓXIMOS PASOS

1. **En el otro computador:**
   - Crear archivo `.env` con la configuración de arriba
   - Instalar dependencias: `pip install psycopg2-binary python-dotenv paho-mqtt`
   - Probar conexión con el comando de prueba
   - Ejecutar suscriptor o sensor simulator

2. **Verificar en el servidor:**
   - Ejecutar `consultar_db.py` para ver si llegan datos del otro computador
   - Monitorear logs de PostgreSQL para ver conexiones

---

**Estado Final:** ✅ Sistema completamente configurado y operacional  
**IP del servidor:** 192.168.137.17  
**Puerto PostgreSQL:** 5432  
**Puerto MQTT:** 1883  
**Red:** THETRUTH 4293

**Última verificación:** 18 de Octubre, 2025 - 09:36 CDT
