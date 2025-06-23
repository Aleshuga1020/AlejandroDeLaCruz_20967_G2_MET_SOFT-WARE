
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cstring>

struct Cliente {
    int id;
    std::string nombre;
    std::string telefono;
    std::string email;
    std::string estado; // "activo", "inactivo"
    
    Cliente(int _id, const std::string& _nombre, const std::string& _telefono, 
            const std::string& _email) 
        : id(_id), nombre(_nombre), telefono(_telefono), email(_email), estado("activo") {}
};

struct Mensaje {
    int id;
    std::string contenido;
    std::string tipo; // "promocional", "recordatorio", "bienvenida"
    std::time_t fechaCreacion;
    
    Mensaje(int _id, const std::string& _contenido, const std::string& _tipo)
        : id(_id), contenido(_contenido), tipo(_tipo) {
        fechaCreacion = std::time(nullptr);
    }
};

struct MensajeEnviado {
    int clienteId;
    int mensajeId;
    std::time_t fechaEnvio;
    std::string estado; // "enviado", "pendiente", "fallido"
    
    MensajeEnviado(int _clienteId, int _mensajeId, const std::string& _estado)
        : clienteId(_clienteId), mensajeId(_mensajeId), estado(_estado) {
        fechaEnvio = std::time(nullptr);
    }
};

class SistemaMensajeria {
private:
    std::vector<Cliente> clientes;
    std::vector<Mensaje> plantillas;
    std::vector<MensajeEnviado> historial;
    int proximoIdCliente;
    int proximoIdMensaje;

public:
    SistemaMensajeria() : proximoIdCliente(1), proximoIdMensaje(1) {
        inicializarPlantillas();
    }

    void inicializarPlantillas() {
        plantillas.push_back(Mensaje(proximoIdMensaje++, 
            "¡Bienvenido a nuestros servicios! Gracias por confiar en nosotros.", 
            "bienvenida"));
        
        plantillas.push_back(Mensaje(proximoIdMensaje++, 
            "¡Oferta especial! 20% de descuento en todos nuestros productos. ¡No te lo pierdas!", 
            "promocional"));
        
        plantillas.push_back(Mensaje(proximoIdMensaje++, 
            "Recordatorio: Tienes una cita pendiente con nosotros. ¡Te esperamos!", 
            "recordatorio"));
        
        plantillas.push_back(Mensaje(proximoIdMensaje++, 
            "Gracias por tu compra. Tu pedido está siendo procesado.", 
            "confirmacion"));
    }

    void agregarCliente() {
        std::string nombre, telefono, email;
        
        std::cout << "\n=== AGREGAR NUEVO CLIENTE ===\n";
        std::cout << "Nombre: ";
        std::cin.ignore();
        std::getline(std::cin, nombre);
        
        std::cout << "Teléfono: ";
        std::getline(std::cin, telefono);
        
        std::cout << "Email: ";
        std::getline(std::cin, email);
        
        clientes.push_back(Cliente(proximoIdCliente++, nombre, telefono, email));
        std::cout << "Cliente agregado exitosamente con ID: " << (proximoIdCliente - 1) << std::endl;
    }

    void listarClientes() {
        std::cout << "\n=== LISTA DE CLIENTES ===\n";
        if (clientes.empty()) {
            std::cout << "No hay clientes registrados.\n";
            return;
        }

        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(20) << "Nombre" 
                  << std::setw(15) << "Teléfono" 
                  << std::setw(25) << "Email" 
                  << std::setw(10) << "Estado" << std::endl;
        std::cout << std::string(75, '-') << std::endl;

        for (const auto& cliente : clientes) {
            std::cout << std::left << std::setw(5) << cliente.id
                      << std::setw(20) << cliente.nombre
                      << std::setw(15) << cliente.telefono
                      << std::setw(25) << cliente.email
                      << std::setw(10) << cliente.estado << std::endl;
        }
    }

    void listarPlantillas() {
        std::cout << "\n=== PLANTILLAS DE MENSAJES ===\n";
        for (const auto& plantilla : plantillas) {
            std::cout << "ID: " << plantilla.id << " | Tipo: " << plantilla.tipo << std::endl;
            std::cout << "Contenido: " << plantilla.contenido << std::endl;
            std::cout << std::string(50, '-') << std::endl;
        }
    }

    void enviarMensajeIndividual() {
        if (clientes.empty()) {
            std::cout << "No hay clientes registrados.\n";
            return;
        }

        listarClientes();
        
        int clienteId;
        std::cout << "\nIngrese el ID del cliente: ";
        std::cin >> clienteId;

        auto clienteIt = std::find_if(clientes.begin(), clientes.end(),
            [clienteId](const Cliente& c) { return c.id == clienteId; });

        if (clienteIt == clientes.end()) {
            std::cout << "Cliente no encontrado.\n";
            return;
        }

        listarPlantillas();
        
        int plantillaId;
        std::cout << "\nIngrese el ID de la plantilla: ";
        std::cin >> plantillaId;

        auto plantillaIt = std::find_if(plantillas.begin(), plantillas.end(),
            [plantillaId](const Mensaje& m) { return m.id == plantillaId; });

        if (plantillaIt == plantillas.end()) {
            std::cout << "Plantilla no encontrada.\n";
            return;
        }

        // Simular envío de mensaje
        historial.push_back(MensajeEnviado(clienteId, plantillaId, "enviado"));
        
        std::cout << "\n=== MENSAJE ENVIADO ===\n";
        std::cout << "Para: " << clienteIt->nombre << " (" << clienteIt->telefono << ")\n";
        std::cout << "Contenido: " << plantillaIt->contenido << std::endl;
        std::cout << "Estado: Enviado exitosamente\n";
    }

    void enviarMensajeMasivo() {
        if (clientes.empty()) {
            std::cout << "No hay clientes registrados.\n";
            return;
        }

        listarPlantillas();
        
        int plantillaId;
        std::cout << "\nIngrese el ID de la plantilla para envío masivo: ";
        std::cin >> plantillaId;

        auto plantillaIt = std::find_if(plantillas.begin(), plantillas.end(),
            [plantillaId](const Mensaje& m) { return m.id == plantillaId; });

        if (plantillaIt == plantillas.end()) {
            std::cout << "Plantilla no encontrada.\n";
            return;
        }

        int clientesEnviados = 0;
        std::cout << "\n=== ENVIANDO MENSAJES MASIVOS ===\n";
        
        for (const auto& cliente : clientes) {
            if (cliente.estado == "activo") {
                historial.push_back(MensajeEnviado(cliente.id, plantillaId, "enviado"));
                std::cout << "Enviado a: " << cliente.nombre << " (" << cliente.telefono << ")\n";
                clientesEnviados++;
            }
        }

        std::cout << "\nTotal de mensajes enviados: " << clientesEnviados << std::endl;
    }

    void verHistorial() {
        std::cout << "\n=== HISTORIAL DE MENSAJES ===\n";
        if (historial.empty()) {
            std::cout << "No hay mensajes en el historial.\n";
            return;
        }

        for (const auto& mensaje : historial) {
            auto clienteIt = std::find_if(clientes.begin(), clientes.end(),
                [mensaje](const Cliente& c) { return c.id == mensaje.clienteId; });
            
            auto plantillaIt = std::find_if(plantillas.begin(), plantillas.end(),
                [mensaje](const Mensaje& m) { return m.id == mensaje.mensajeId; });

            if (clienteIt != clientes.end() && plantillaIt != plantillas.end()) {
                std::cout << "Cliente: " << clienteIt->nombre;
                std::cout << " | Tipo: " << plantillaIt->tipo;
                std::cout << " | Estado: " << mensaje.estado;
                
                char* timeStr = std::ctime(&mensaje.fechaEnvio);
                timeStr[strlen(timeStr) - 1] = '\0'; // Remover el salto de línea
                std::cout << " | Fecha: " << timeStr << std::endl;
            }
        }
    }

    void crearPlantilla() {
        std::string contenido, tipo;
        
        std::cout << "\n=== CREAR NUEVA PLANTILLA ===\n";
        std::cout << "Tipo de mensaje (bienvenida/promocional/recordatorio/confirmacion): ";
        std::cin >> tipo;
        
        std::cout << "Contenido del mensaje: ";
        std::cin.ignore();
        std::getline(std::cin, contenido);
        
        plantillas.push_back(Mensaje(proximoIdMensaje++, contenido, tipo));
        std::cout << "Plantilla creada exitosamente con ID: " << (proximoIdMensaje - 1) << std::endl;
    }

    void mostrarEstadisticas() {
        std::cout << "\n=== ESTADÍSTICAS DEL SISTEMA ===\n";
        std::cout << "Total de clientes: " << clientes.size() << std::endl;
        
        int clientesActivos = std::count_if(clientes.begin(), clientes.end(),
            [](const Cliente& c) { return c.estado == "activo"; });
        std::cout << "Clientes activos: " << clientesActivos << std::endl;
        
        std::cout << "Total de plantillas: " << plantillas.size() << std::endl;
        std::cout << "Mensajes enviados: " << historial.size() << std::endl;
        
        // Estadísticas por tipo de mensaje
        std::map<std::string, int> tiposMensaje;
        for (const auto& mensaje : historial) {
            auto plantillaIt = std::find_if(plantillas.begin(), plantillas.end(),
                [mensaje](const Mensaje& m) { return m.id == mensaje.mensajeId; });
            if (plantillaIt != plantillas.end()) {
                tiposMensaje[plantillaIt->tipo]++;
            }
        }
        
        std::cout << "\nMensajes por tipo:\n";
        for (const auto& par : tiposMensaje) {
            std::cout << "- " << par.first << ": " << par.second << std::endl;
        }
    }

    bool autenticar() {
        std::string usuario, contrasena;
        int intentos = 0;
        const int maxIntentos = 3;
        
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "    SISTEMA DE MENSAJERÍA AUTOMÁTICA" << std::endl;
        std::cout << "           INICIO DE SESIÓN" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        while (intentos < maxIntentos) {
            std::cout << "\nUsuario: ";
            std::cin >> usuario;
            
            std::cout << "Contraseña: ";
            std::cin >> contrasena;
            
            if (usuario == "administrador" && contrasena == "administrador001") {
                std::cout << "\n¡Acceso concedido! Bienvenido al sistema.\n";
                return true;
            } else {
                intentos++;
                std::cout << "\nCredenciales incorrectas. ";
                if (intentos < maxIntentos) {
                    std::cout << "Intento " << intentos << " de " << maxIntentos << ".\n";
                } else {
                    std::cout << "Acceso denegado. Has excedido el número máximo de intentos.\n";
                }
            }
        }
        
        return false;
    }

    void ejecutar() {
        // Verificar autenticación antes de mostrar el menú
        if (!autenticar()) {
            std::cout << "Cerrando el sistema...\n";
            return;
        }
        
        int opcion;
        
        do {
            std::cout << "\n" << std::string(50, '=') << std::endl;
            std::cout << "    SISTEMA DE MENSAJERÍA AUTOMÁTICA" << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            std::cout << "1. Agregar cliente\n";
            std::cout << "2. Listar clientes\n";
            std::cout << "3. Enviar mensaje individual\n";
            std::cout << "4. Enviar mensaje masivo\n";
            std::cout << "5. Ver plantillas de mensajes\n";
            std::cout << "6. Crear nueva plantilla\n";
            std::cout << "7. Ver historial de mensajes\n";
            std::cout << "8. Ver estadísticas\n";
            std::cout << "0. Salir\n";
            std::cout << std::string(50, '-') << std::endl;
            std::cout << "Seleccione una opción: ";
            
            std::cin >> opcion;
            
            switch (opcion) {
                case 1:
                    agregarCliente();
                    break;
                case 2:
                    listarClientes();
                    break;
                case 3:
                    enviarMensajeIndividual();
                    break;
                case 4:
                    enviarMensajeMasivo();
                    break;
                case 5:
                    listarPlantillas();
                    break;
                case 6:
                    crearPlantilla();
                    break;
                case 7:
                    verHistorial();
                    break;
                case 8:
                    mostrarEstadisticas();
                    break;
                case 0:
                    std::cout << "¡Gracias por usar el sistema de mensajería!\n";
                    break;
                default:
                    std::cout << "Opción no válida. Intente de nuevo.\n";
            }
            
            if (opcion != 0) {
                std::cout << "\nPresione Enter para continuar...";
                std::cin.ignore();
                std::cin.get();
            }
            
        } while (opcion != 0);
    }
};

int main() {
    std::cout << "Iniciando Sistema de Mensajería Automática para Clientes...\n";
    
    SistemaMensajeria sistema;
    sistema.ejecutar();
    
    return 0;
}
