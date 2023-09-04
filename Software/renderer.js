const { SerialPort } = require('serialport')
const tableify = require('tableify')
const inputField = document.getElementById("myTextField");
const sendButton = document.getElementById("sendButton");


inputField.addEventListener('keydown', function (event) {
    let keys = [];

    if (event.ctrlKey) keys.push('Ctrl');
    if (event.metaKey) keys.push('Cmd');
    if (event.shiftKey) keys.push('Shift');
    if (event.altKey) keys.push('Option');
    if (event.key === 'Tab') keys.push('Tab');

    if (event.key !== 'Control' && event.key !== 'Meta' && event.key !== 'Shift' && event.key !== 'Alt' && event.key !== 'Tab') {
        keys.push(event.key);
    }

    if (keys.length > 1) {
        inputField.value = keys.join(' + ');
    } else {
        inputField.value = "";
    }
});

async function listSerialPorts() {
    await SerialPort.list().then((ports, err) => {
        if (err) {
            document.getElementById('error').textContent = err.message
            return
        } else {
            document.getElementById('error').textContent = ''
        }
        console.log('ports', ports);

        const serialPortDropdown = document.getElementById('serialPortDropdown');
        ports.forEach((port) => {
            const option = document.createElement('option');
            option.value = port.path;
            option.textContent = port.path;
            serialPortDropdown.appendChild(option);
        })

        if (ports.length === 0) {
            document.getElementById('error').textContent = 'No ports discovered'
        }
    })
}

listSerialPorts()