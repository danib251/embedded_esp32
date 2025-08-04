let serverRunning = false
let connected = false
const startTime = Date.now()
let uptimeInterval

async function makeRequest(endpoint, method = "GET", data = null) {
    try {
        const options = { method }
        if (data) {
            options.headers = { "Content-Type": "application/json" }
            options.body = JSON.stringify(data)
        }
        const response = await fetch(endpoint, options)
        return await response.json()
    } catch (error) {
        console.error("Request failed:", error)
        return { success: false, error: error.message }
    }
}

async function checkConnection() {
    const result = await makeRequest("/api/status")
    updateConnectionStatus(result.success)
    if (result.success) {
        updateServerInfo(result.data)
    }
}

function updateConnectionStatus(isConnected) {
    connected = isConnected
    const statusEl = document.getElementById("connectionStatus")
    const textEl = document.getElementById("connectionText")

    if (isConnected) {
        statusEl.className = "status-indicator status-connected"
        textEl.textContent = "Connected to ESP32"
    } else {
        statusEl.className = "status-indicator status-disconnected"
        textEl.textContent = "Disconnected"
    }
}

function updateServerStatus(running) {
    serverRunning = running
    const statusEl = document.getElementById("serverStatus")
    const textEl = document.getElementById("serverText")
    const startBtn = document.getElementById("startBtn")
    const stopBtn = document.getElementById("stopBtn")

    if (running) {
        statusEl.className = "status-indicator status-connected"
        textEl.textContent = "Server Status: Running"
        startBtn.disabled = true
        stopBtn.disabled = false
    } else {
        statusEl.className = "status-indicator status-disconnected"
        textEl.textContent = "Server Status: Stopped"
        startBtn.disabled = false
        stopBtn.disabled = true
    }
}

function updateServerInfo(data) {
    if (data) {
        document.getElementById("connectionCount").textContent = data.connections || 0
        document.getElementById("currentPort").textContent = data.port || 3333
        updateServerStatus(data.tcp_running || false)
    }
}

function showAlert(message, type = "success") {
    const alertEl = document.getElementById("serverAlert")
    alertEl.innerHTML = `<div class='alert alert-${type}'>${message}</div>`
    setTimeout(() => {
        alertEl.innerHTML = ""
    }, 5000)
}

async function startServer() {
    const result = await makeRequest("/api/server/start", "POST")
    if (result.success) {
        showAlert("Server started successfully!", "success")
        updateServerStatus(true)
    } else {
        showAlert("Failed to start server: " + (result.error || "Unknown error"), "error")
    }
}

async function stopServer() {
    const result = await makeRequest("/api/server/stop", "POST")
    if (result.success) {
        showAlert("Server stopped successfully!", "warning")
        updateServerStatus(false)
    } else {
        showAlert("Failed to stop server: " + (result.error || "Unknown error"), "error")
    }
}

async function getStatus() {
    await checkConnection()
}

async function updateWiFi() {
    const ssid = document.getElementById("wifiSSID").value
    const password = document.getElementById("wifiPassword").value

    if (!ssid || !password) {
        showAlert("Please enter both SSID and password", "error")
        return
    }

    const result = await makeRequest("/api/wifi/config", "POST", { ssid, password })
    if (result.success) {
        showAlert("WiFi configuration updated! Device will restart.", "success")
    } else {
        showAlert("Failed to update WiFi: " + (result.error || "Unknown error"), "error")
    }
}

// Auto-refresh status every 5 seconds
setInterval(checkConnection, 5000)

// Initial status check
checkConnection()

// Inicializar cuando carga la página
document.addEventListener("DOMContentLoaded", () => {
    initializePortfolio()
    startUptimeCounter()
    loadSystemInfo()
    setupNavigation()
})

// Inicializar el portafolio
function initializePortfolio() {
    console.log("🚀 ESP32 Portfolio iniciado")

    // Animar el LED de estado
    const statusLed = document.getElementById("statusLed")
    if (statusLed) {
        statusLed.style.background = "#27ae60" // Verde para indicar online
    }
}

// Contador de tiempo online
function startUptimeCounter() {
    uptimeInterval = setInterval(() => {
        const uptime = Math.floor((Date.now() - startTime) / 1000)
        const uptimeElement = document.getElementById("uptime")
        if (uptimeElement) {
            uptimeElement.textContent = uptime.toLocaleString()
        }
    }, 1000)
}

// Cargar información del sistema
async function loadSystemInfo() {
    try {
        // Simular carga de IP (en el ESP32 real, esto vendría de una API)
        const ipElement = document.getElementById("ipAddress")
        if (ipElement) {
            // En el ESP32 real, esto sería una llamada a /api/status
            ipElement.textContent = window.location.hostname || "192.168.1.100"
        }

        const statusElement = document.getElementById("systemStatus")
        if (statusElement) {
            statusElement.textContent = "Online ✅"
            statusElement.style.color = "#27ae60"
        }
    } catch (error) {
        console.error("Error cargando info del sistema:", error)
    }
}

// Configurar navegación suave
function setupNavigation() {
    const navLinks = document.querySelectorAll(".nav-link")

    navLinks.forEach((link) => {
        link.addEventListener("click", function (e) {
            e.preventDefault()

            // Remover clase active de todos los links
            navLinks.forEach((l) => l.classList.remove("active"))

            // Agregar clase active al link clickeado
            this.classList.add("active")

            // Scroll suave a la sección
            const targetId = this.getAttribute("href").substring(1)
            const targetSection = document.getElementById(targetId)

            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: "smooth",
                    block: "start",
                })
            }
        })
    })
}

// Mostrar modal de información
function showInfo() {
    const modal = document.getElementById("infoModal")
    if (modal) {
        modal.style.display = "block"

        // Animar entrada
        const modalContent = modal.querySelector(".modal-content")
        modalContent.style.transform = "scale(0.7)"
        modalContent.style.opacity = "0"

        setTimeout(() => {
            modalContent.style.transition = "all 0.3s ease"
            modalContent.style.transform = "scale(1)"
            modalContent.style.opacity = "1"
        }, 10)
    }
}

// Cerrar modal
function closeModal() {
    const modal = document.getElementById("infoModal")
    if (modal) {
        const modalContent = modal.querySelector(".modal-content")
        modalContent.style.transform = "scale(0.7)"
        modalContent.style.opacity = "0"

        setTimeout(() => {
            modal.style.display = "none"
        }, 300)
    }
}

// Cerrar modal al hacer click fuera
window.addEventListener("click", (event) => {
    const modal = document.getElementById("infoModal")
    if (event.target === modal) {
        closeModal()
    }
})

// Efectos de scroll
window.addEventListener("scroll", () => {
    const sections = document.querySelectorAll(".section")
    const navLinks = document.querySelectorAll(".nav-link")

    let current = ""

    sections.forEach((section) => {
        const sectionTop = section.offsetTop
        const sectionHeight = section.clientHeight

        if (scrollY >= sectionTop - 200) {
            current = section.getAttribute("id")
        }
    })

    navLinks.forEach((link) => {
        link.classList.remove("active")
        if (link.getAttribute("href").substring(1) === current) {
            link.classList.add("active")
        }
    })
})

// Funciones para integración con ESP32
async function getESP32Status() {
    try {
        const response = await fetch("/api/status")
        const data = await response.json()
        return data
    } catch (error) {
        console.error("Error obteniendo estado del ESP32:", error)
        return null
    }
}

// Función para mostrar notificaciones
function showNotification(message, type = "info") {
    const notification = document.createElement("div")
    notification.className = `notification notification-${type}`
    notification.textContent = message

    notification.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        padding: 1rem 2rem;
        background: ${type === "success" ? "#27ae60" : type === "error" ? "#e74c3c" : "#3498db"};
        color: white;
        border-radius: 5px;
        z-index: 1001;
        transform: translateX(100%);
        transition: transform 0.3s ease;
    `

    document.body.appendChild(notification)

    setTimeout(() => {
        notification.style.transform = "translateX(0)"
    }, 10)

    setTimeout(() => {
        notification.style.transform = "translateX(100%)"
        setTimeout(() => {
            document.body.removeChild(notification)
        }, 300)
    }, 3000)
}

// Log de inicio
console.log(`
🔧 ESP32 Portfolio v1.0
📡 Sistema iniciado correctamente
🌐 Servidor web activo
⚡ Listo para proyectos increíbles!
`)
