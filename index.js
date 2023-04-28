import express, {
    request,
    response
} from "express";
import {
    Server
} from "socket.io";
import cors from "cors";

//Serial Port Configuration
import {
    SerialPort,
    ReadlineParser
} from "serialport"
const protocolConfiguration = {
    path: 'COM8',
    baudRate: 9600
}
const serialPort = new SerialPort(protocolConfiguration);
const parser = serialPort.pipe(new ReadlineParser());


const PORT = 8080
const expressApp = express()
const httpServer = expressApp.listen(PORT, () => {
    console.table({
        'Game': `http://localhost:${PORT}/game`,
    })

})
const io = new Server(httpServer, {
    path: '/real-time'
})

expressApp.use('/game', express.static('public-game'))
expressApp.use(express.json())

io.on('connection', (socket) => {
    console.log('Connected!', socket.id)
})

let currentScore = 0;

expressApp.get('/final-score', (request, response) => {
    response.send({
        content: currentScore
    });
})

expressApp.post('/score', (request, response) =>{
    console.log(request.body);
    serialPort.write('S');
    response.end();
})


/*___________________________________________

2) Create an endpoint to POST that the game is over and turn on all the lights.
_____________________________________________ */

expressApp.post('/game-over', (request, response) => {
    console.log(response.content);
    serialPort.write('L');
    response.end();
})


let arduinoMessage = {
    actuatorValue: 0,
    btnAValue: 0,
    btnBValue: 0
}

parser.on('data', (data) => {
    console.log(data);
    let dataArray = data.split(' ')
    arduinoMessage.actuatorValue = parseInt(dataArray[0])
    arduinoMessage.btnAValue = parseInt(dataArray[1])
    arduinoMessage.btnBValue = parseInt(dataArray[2])

    io.emit('arduinoMessage', arduinoMessage);
    console.log(arduinoMessage);
});