#!/usr/bin/env python3
"""
Script para Limpiar Base de Datos MQTT
Uso: python limpiar_db.py
o desde el venv: .venv/bin/python limpiar_db.py
"""

import sys
from database.db_config import crear_conexion

def limpiar_base_datos():
    """Borra todos los datos de la tabla mensajes_mqtt"""
    try:
        print("=" * 70)
        print("🗑️  LIMPIEZA DE BASE DE DATOS - SISTEMA MQTT")
        print("=" * 70)
        print()
        
        # Confirmar acción
        print("⚠️  ADVERTENCIA: Esta acción borrará TODOS los mensajes de la base de datos.")
        print("Esta acción NO se puede deshacer.")
        print()
        
        respuesta = input("¿Estás seguro de que deseas continuar? (escribe 'SI' para confirmar): ")
        
        if respuesta.upper() != 'SI':
            print("\n❌ Operación cancelada por el usuario")
            return False
        
        print()
        print("Conectando a la base de datos...")
        
        conn = crear_conexion()
        cursor = conn.cursor()
        
        # Contar mensajes antes de borrar
        cursor.execute("SELECT COUNT(*) FROM mensajes_mqtt;")
        total_antes = cursor.fetchone()[0]
        print(f"📊 Mensajes actuales en la base de datos: {total_antes}")
        
        if total_antes == 0:
            print("\n✅ La base de datos ya está vacía. No hay nada que borrar.")
            cursor.close()
            conn.close()
            return True
        
        print()
        print("🗑️  Borrando todos los mensajes...")
        
        # Borrar todos los datos de la tabla
        cursor.execute("DELETE FROM mensajes_mqtt;")
        
        # Confirmar cambios
        conn.commit()
        
        # Verificar que se borraron
        cursor.execute("SELECT COUNT(*) FROM mensajes_mqtt;")
        total_despues = cursor.fetchone()[0]
        
        print(f"✅ Se borraron {total_antes} mensajes exitosamente")
        print(f"📊 Mensajes actuales en la base de datos: {total_despues}")
        
        # Resetear secuencia de ID (opcional)
        print()
        print("🔄 Reseteando secuencia de IDs...")
        cursor.execute("ALTER SEQUENCE mensajes_mqtt_id_seq RESTART WITH 1;")
        conn.commit()
        print("✅ Secuencia de IDs reseteada")
        
        cursor.close()
        conn.close()
        
        print()
        print("=" * 70)
        print("✅ BASE DE DATOS LIMPIADA EXITOSAMENTE")
        print("=" * 70)
        print()
        
        return True
        
    except Exception as e:
        print(f"\n❌ Error al limpiar la base de datos: {e}")
        import traceback
        traceback.print_exc()
        return False

if __name__ == "__main__":
    try:
        limpiar_base_datos()
    except KeyboardInterrupt:
        print("\n\n⚠️  Operación cancelada por el usuario")
        sys.exit(1)
