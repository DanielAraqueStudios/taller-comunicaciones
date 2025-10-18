#!/usr/bin/env python3
"""
Script para Configurar PostgreSQL para Conexiones Remotas
Universidad Militar Nueva Granada - Taller de Comunicaciones

Este script configura PostgreSQL para aceptar conexiones desde otros computadores
en la red local.
"""

import subprocess
import sys
import os

def print_header(text):
    print(f"\n{'='*70}")
    print(f"{text.center(70)}")
    print(f"{'='*70}\n")

def print_step(text):
    print(f"🔧 {text}")

def print_success(text):
    print(f"✅ {text}")

def print_error(text):
    print(f"❌ {text}")

def print_info(text):
    print(f"ℹ️  {text}")

def run_command(command):
    """Ejecuta un comando y retorna el resultado"""
    try:
        result = subprocess.run(
            command,
            shell=True,
            check=True,
            capture_output=True,
            text=True
        )
        return True, result.stdout
    except subprocess.CalledProcessError as e:
        return False, e.stderr

def get_local_ip():
    """Obtiene la IP local del servidor"""
    success, output = run_command("ip addr show | grep -E 'inet.*192.168' | head -1 | awk '{print $2}' | cut -d'/' -f1")
    if success and output.strip():
        return output.strip()
    return None

def configure_postgresql_conf():
    """Configura postgresql.conf para escuchar en todas las interfaces"""
    print_step("Configurando postgresql.conf...")
    
    # Ubicación del archivo postgresql.conf
    pg_version_cmd = "ls /etc/postgresql/ | head -1"
    success, version = run_command(pg_version_cmd)
    
    if not success or not version.strip():
        print_error("No se pudo detectar la versión de PostgreSQL")
        return False
    
    version = version.strip()
    conf_file = f"/etc/postgresql/{version}/main/postgresql.conf"
    
    print_info(f"Archivo de configuración: {conf_file}")
    
    # Backup del archivo original
    backup_cmd = f"sudo cp {conf_file} {conf_file}.backup"
    run_command(backup_cmd)
    print_info("Backup creado")
    
    # Modificar listen_addresses
    modify_cmd = f"""sudo sed -i "s/#listen_addresses = 'localhost'/listen_addresses = '*'/g" {conf_file}"""
    success, output = run_command(modify_cmd)
    
    if success:
        print_success("postgresql.conf configurado para escuchar en todas las interfaces")
        return True
    else:
        print_error(f"Error al modificar postgresql.conf: {output}")
        return False

def configure_pg_hba():
    """Configura pg_hba.conf para permitir conexiones remotas"""
    print_step("Configurando pg_hba.conf...")
    
    # Ubicación del archivo pg_hba.conf
    pg_version_cmd = "ls /etc/postgresql/ | head -1"
    success, version = run_command(pg_version_cmd)
    
    if not success or not version.strip():
        print_error("No se pudo detectar la versión de PostgreSQL")
        return False
    
    version = version.strip()
    hba_file = f"/etc/postgresql/{version}/main/pg_hba.conf"
    
    print_info(f"Archivo de configuración: {hba_file}")
    
    # Backup del archivo original
    backup_cmd = f"sudo cp {hba_file} {hba_file}.backup"
    run_command(backup_cmd)
    print_info("Backup creado")
    
    # Agregar regla para permitir conexiones desde la red local
    # Permitir conexiones desde 192.168.137.0/24
    add_rule_cmd = f"""sudo bash -c 'echo "" >> {hba_file}'"""
    run_command(add_rule_cmd)
    
    add_rule_cmd = f"""sudo bash -c 'echo "# Permitir conexiones remotas desde la red local" >> {hba_file}'"""
    run_command(add_rule_cmd)
    
    add_rule_cmd = f"""sudo bash -c 'echo "host    mqtt_taller    mqtt_admin    192.168.137.0/24    md5" >> {hba_file}'"""
    success, output = run_command(add_rule_cmd)
    
    if success:
        print_success("pg_hba.conf configurado para permitir conexiones remotas")
        return True
    else:
        print_error(f"Error al modificar pg_hba.conf: {output}")
        return False

def configure_firewall():
    """Configura el firewall para permitir conexiones al puerto 5432"""
    print_step("Configurando firewall...")
    
    # Verificar si UFW está instalado
    success, output = run_command("which ufw")
    
    if success:
        # Permitir puerto 5432
        allow_cmd = "sudo ufw allow 5432/tcp"
        success, output = run_command(allow_cmd)
        
        if success:
            print_success("Firewall configurado (puerto 5432 abierto)")
            return True
        else:
            print_info("UFW no está activo o no se pudo configurar")
            return True
    else:
        print_info("UFW no está instalado (el puerto debería estar abierto)")
        return True

def restart_postgresql():
    """Reinicia el servicio PostgreSQL"""
    print_step("Reiniciando PostgreSQL...")
    
    restart_cmd = "sudo systemctl restart postgresql"
    success, output = run_command(restart_cmd)
    
    if success:
        print_success("PostgreSQL reiniciado exitosamente")
        return True
    else:
        print_error(f"Error al reiniciar PostgreSQL: {output}")
        return False

def verify_configuration():
    """Verifica la configuración"""
    print_step("Verificando configuración...")
    
    # Verificar que PostgreSQL esté escuchando en el puerto 5432
    netstat_cmd = "sudo netstat -tuln | grep 5432 || sudo ss -tuln | grep 5432"
    success, output = run_command(netstat_cmd)
    
    if success and "5432" in output:
        print_success("PostgreSQL está escuchando en el puerto 5432")
        
        # Verificar si está escuchando en todas las interfaces
        if "0.0.0.0:5432" in output or "*:5432" in output:
            print_success("PostgreSQL está escuchando en TODAS las interfaces")
        else:
            print_info("PostgreSQL puede estar escuchando solo en localhost")
        
        print_info(f"Detalles:\n{output}")
        return True
    else:
        print_error("PostgreSQL no está escuchando en el puerto 5432")
        return False

def print_instructions(server_ip):
    """Imprime las instrucciones finales"""
    print_header("CONFIGURACIÓN COMPLETADA")
    
    print(f"✅ PostgreSQL está configurado para aceptar conexiones remotas\n")
    
    print("📋 CONFIGURACIÓN EN EL OTRO COMPUTADOR:\n")
    print("1. Archivo .env debe contener:")
    print(f"   DB_HOST={server_ip}")
    print("   DB_PORT=5432")
    print("   DB_NAME=mqtt_taller")
    print("   DB_USER=mqtt_admin")
    print("   DB_PASSWORD=mqtt_secure_2025")
    
    print("\n2. Instalar dependencias en el otro computador:")
    print("   python3 -m venv .venv")
    print("   source .venv/bin/activate")
    print("   pip install psycopg2-binary python-dotenv paho-mqtt")
    
    print("\n3. Probar conexión desde el otro computador:")
    print(f"   psql -h {server_ip} -U mqtt_admin -d mqtt_taller")
    print("   (Contraseña: mqtt_secure_2025)")
    
    print("\n4. O usar Python:")
    print("   python -c \"import psycopg2; conn = psycopg2.connect(")
    print(f"       host='{server_ip}', database='mqtt_taller',")
    print("       user='mqtt_admin', password='mqtt_secure_2025');")
    print("       print('✅ Conexión exitosa'); conn.close()\"")
    
    print("\n⚠️  IMPORTANTE:")
    print(f"   - IP del servidor: {server_ip}")
    print("   - Puerto: 5432")
    print("   - Ambos computadores deben estar en la misma red (THETRUTH 4293)")
    print("   - El firewall debe permitir el puerto 5432")
    
    print("\n" + "="*70 + "\n")

def main():
    print_header("CONFIGURACIÓN DE POSTGRESQL REMOTO")
    print("Universidad Militar Nueva Granada")
    print("Taller de Comunicaciones\n")
    
    # Obtener IP local
    server_ip = get_local_ip()
    if not server_ip:
        print_error("No se pudo detectar la IP del servidor")
        sys.exit(1)
    
    print_info(f"IP del servidor PostgreSQL: {server_ip}\n")
    
    # Confirmar
    print("Este script configurará PostgreSQL para aceptar conexiones remotas.")
    print("Esto permitirá que otros computadores en la red se conecten a la base de datos.")
    
    response = input("\n¿Deseas continuar? (s/n): ").lower()
    if response != 's':
        print("Operación cancelada")
        sys.exit(0)
    
    # Ejecutar configuración
    steps = [
        configure_postgresql_conf,
        configure_pg_hba,
        configure_firewall,
        restart_postgresql,
        verify_configuration
    ]
    
    for step in steps:
        if not step():
            print_error("Error en la configuración. Revisa los mensajes arriba.")
            sys.exit(1)
    
    # Instrucciones finales
    print_instructions(server_ip)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n⚠️  Operación cancelada por el usuario")
        sys.exit(1)
    except Exception as e:
        print_error(f"Error inesperado: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
