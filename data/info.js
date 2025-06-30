
class SimpleWebSerial {
    constructor() {
        this.port = null;
        this.reader = null;
        this.writer = null;
        this.isConnected = false;
        this.baudRate = 9600;
        this.autoScroll = true;
        
        this.initElements();
        this.bindEvents();
        this.checkSupport();
    }

    initElements() {
        this.connectBtn = document.getElementById('connectBtn');
        this.sendBtn = document.getElementById('sendBtn');
        this.messageInput = document.getElementById('messageInput');
        this.terminal = document.getElementById('terminal');
        this.connectionStatus = document.getElementById('connectionStatus');
        this.statusText = document.getElementById('statusText');
        this.clearBtn = document.getElementById('clearBtn');
        this.autoScrollBtn = document.getElementById('autoScrollBtn');
        this.baudButtons = document.querySelectorAll('[data-baud]');
    }

    bindEvents() {
        this.connectBtn.addEventListener('click', () => this.toggleConnection());
        this.sendBtn.addEventListener('click', () => this.sendMessage());
        this.clearBtn.addEventListener('click', () => this.clearTerminal());
        this.autoScrollBtn.addEventListener('click', () => this.toggleAutoScroll());
        
        this.messageInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.sendMessage();
            }
        });

        this.baudButtons.forEach(btn => {
            btn.addEventListener('click', () => {
                if (!this.isConnected) {
                    this.baudButtons.forEach(b => b.classList.remove('active'));
                    btn.classList.add('active');
                    this.baudRate = parseInt(btn.dataset.baud);
                }
            });
        });
    }

    checkSupport() {
        if (!('serial' in navigator)) {
            this.addMessage('WebSerial API not supported in this browser', 'system');
            this.connectBtn.disabled = true;
        } else {
            this.addMessage('WebSerial API ready', 'system');
        }
    }

    async toggleConnection() {
        if (this.isConnected) {
            await this.disconnect();
        } else {
            await this.connect();
        }
    }

    async connect() {
        try {
            this.addMessage('Requesting serial port...', 'system');
            
            this.port = await navigator.serial.requestPort();
            await this.port.open({ baudRate: this.baudRate });

            this.isConnected = true;
            this.updateConnectionStatus();
            this.addMessage(`Connected at ${this.baudRate} baud`, 'system');
            
            this.startReading();
            
        } catch (error) {
            this.addMessage(`Connection failed: ${error.message}`, 'system');
        }
    }

    async disconnect() {
        try {
            if (this.reader) {
                await this.reader.cancel();
                this.reader.releaseLock();
            }
            
            if (this.writer) {
                this.writer.releaseLock();
            }
            
            if (this.port) {
                await this.port.close();
            }
            
            this.isConnected = false;
            this.updateConnectionStatus();
            this.addMessage('Disconnected', 'system');
            
        } catch (error) {
            this.addMessage(`Disconnect error: ${error.message}`, 'system');
        }
    }

    async startReading() {
        try {
            this.reader = this.port.readable.getReader();
            
            while (this.isConnected) {
                const { value, done } = await this.reader.read();
                if (done) break;
                
                const text = new TextDecoder().decode(value);
                this.addMessage(text.trim(), 'received');
            }
            
        } catch (error) {
            if (this.isConnected) {
                this.addMessage(`Read error: ${error.message}`, 'system');
            }
        } finally {
            if (this.reader) {
                this.reader.releaseLock();
            }
        }
    }

    async sendMessage() {
        const message = this.messageInput.value.trim();
        if (!message || !this.isConnected) return;

        try {
            if (!this.writer) {
                this.writer = this.port.writable.getWriter();
            }
            
            const data = new TextEncoder().encode(message + '\n');
            await this.writer.write(data);
            
            this.addMessage(message, 'sent');
            this.messageInput.value = '';
            
        } catch (error) {
            this.addMessage(`Send error: ${error.message}`, 'system');
        }
    }

    addMessage(text, type = 'received') {
        // Clear empty state if it exists
        if (this.terminal.querySelector('.empty-state')) {
            this.terminal.innerHTML = '';
        }

        const messageDiv = document.createElement('div');
        messageDiv.className = `terminal-message ${type}`;
        
        const timestamp = new Date().toLocaleTimeString();
        messageDiv.textContent = `[${timestamp}] ${text}`;
        
        this.terminal.appendChild(messageDiv);
        
        if (this.autoScroll) {
            this.terminal.scrollTop = this.terminal.scrollHeight;
        }
        
        // Keep only last 100 messages
        const messages = this.terminal.querySelectorAll('.terminal-message');
        if (messages.length > 100) {
            messages[0].remove();
        }
    }

    updateConnectionStatus() {
        if (this.isConnected) {
            this.connectionStatus.classList.add('connected');
            this.statusText.textContent = 'Connected';
            this.connectBtn.textContent = 'Disconnect';
            this.connectBtn.classList.add('disconnect');
            this.messageInput.disabled = false;
            this.sendBtn.disabled = false;
            this.baudButtons.forEach(btn => btn.style.pointerEvents = 'none');
        } else {
            this.connectionStatus.classList.remove('connected');
            this.statusText.textContent = 'Disconnected';
            this.connectBtn.textContent = 'Connect';
            this.connectBtn.classList.remove('disconnect');
            this.messageInput.disabled = true;
            this.sendBtn.disabled = true;
            this.baudButtons.forEach(btn => btn.style.pointerEvents = 'auto');
        }
    }

    clearTerminal() {
        this.terminal.innerHTML = '<div class="empty-state"><div class="empty-state-icon">🧹</div><div>Terminal cleared</div></div>';
    }

    toggleAutoScroll() {
        this.autoScroll = !this.autoScroll;
        this.autoScrollBtn.classList.toggle('active', this.autoScroll);
        this.autoScrollBtn.textContent = this.autoScroll ? 'Auto-scroll' : 'Manual';
    }
}

// Initialize when page loads
document.addEventListener('DOMContentLoaded', () => {
    new SimpleWebSerial();
});
