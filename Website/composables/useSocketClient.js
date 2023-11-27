import { io } from 'socket.io-client';

export default function () {
  const socket = useState('socket', () => undefined);
  const { addLog, clearLogs } = useLogs();

  // Connecting & Disconnecting from Server
  const { serverConnection } = useStatus();
  const { socketRoom } = useRuntimeConfig().public;
  function connect(host) {
    // Setting `serverConnection` status to 'Connecting'
    serverConnection.value = 2;
    addLog({ type: 'info', message: 'Connecting to server...' });

    // Initializing Socket Connection
    socket.value = io(host);

    // Joining the room
    socket.value.emit('join', socketRoom);

    // Passing event handlers
    useSocketConnectionHandlers(socket);
    useSocketMessageHandlers(socket);

    // Returning socket instance
    return socket;
  }

  function disconnect() {
    socket.value.disconnect();
    clearLogs();
  }

  // Connecting & Disconnecting from Car
  const { carConnection } = useStatus();
  const { carConnectionTimeoutSeconds } = useRuntimeConfig().public;
  function connectToCar(carToken) {
    console.debug('[Car] Connecting...');
    addLog({ type: 'info', message: 'Connecting to car...' });

    // Setting `carConnection` status to 'Connecting'
    carConnection.value = 2;
    socket.value.emit('W_connectCar', carToken);

    // Waiting for timeout period then stop trying to connect
    setTimeout(() => {
      if (carConnection.value !== 2) return;

      carConnection.value = 0;
      const message = `Connecting to car timeout reached (${carConnectionTimeoutSeconds}s).`;
      console.debug(`[Car] ${message}`);
      addLog({ type: 'info', message });
    }, carConnectionTimeoutSeconds * 1000);
  }

  function disconnectFromCar() {
    socket.value.emit('W_disconnectCar');
    console.debug('[Car] Disconnecting...');
    addLog({ type: 'info', message: 'Disconnecting from car...' });
  }

  // Connecting & Disconnecting from ECUs
  const { ecuConnection } = useStatus();
  const { ecuConnectionTimeoutSeconds } = useRuntimeConfig().public;
  function connectToEcu(ecuId) {
    console.debug(`[ECU] Connecting to ECU${ecuId}...`);
    addLog({ type: 'info', message: `Connecting to ECU${ecuId}...` });

    // Setting `ecuConnection` status to 'Connecting'
    ecuConnection.value[`ecu${ecuId}`] = 2;
    socket.value.emit('W_connectECU', `${ecuId}`);

    // Waiting for timeout period then stop trying to connect
    setTimeout(() => {
      if (ecuConnection.value[`ecu${ecuId}`] !== 2) return;

      ecuConnection.value[`ecu${ecuId}`] = 0;
      const message = `Connecting to ecu timeout reached (${ecuConnectionTimeoutSeconds}s).`;
      console.debug(`[ECU] ${message}`);
      addLog({ type: 'info', message });
    }, ecuConnectionTimeoutSeconds * 1000);
  }

  function disconnectFromEcu(ecuId) {
    socket.value.emit('W_disconnectECU', `${ecuId}`);
    console.debug(`[ECU] Disconnecting from ECU${ecuId}...`);
    addLog({ type: 'info', message: `Disconnecting from ECU${ecuId}...` });
  }

  // Sending & Flashing ECUs
  const { fernetKey, verificationTimeoutSeconds } = useRuntimeConfig().public;
  const { hexVerification } = useStatus();
  function sendHexToEcu(hexFile) {
    hexVerification.value = 2;

    // Serialize
    const serializedHex = hexFile.replace(/\r\n/g, ',');

    // Encrypt using Fernet
    const fernetSecret = new fernet.Secret(fernetKey);
    const fernetToken = new fernet.Token({ secret: fernetSecret, ttl: 0 });
    const encryptedHex = fernetToken.encode(serializedHex);

    // Send to the ECU to verify
    socket.value.emit('W_receiveHex', encryptedHex);
    console.debug(`[ECU] Sending file to ecu...`);
    addLog({ type: 'info', message: `Sending file to ecu...` });

    // Waiting for timeout period then stop trying to verify
    setTimeout(() => {
      if (hexVerification.value !== 2) return;

      hexVerification.value = 0;
      const message = `File verification timeout reached (${verificationTimeoutSeconds}s).`;
      console.debug(`[ECU] ${message}`);
      addLog({ type: 'info', message });
    }, verificationTimeoutSeconds * 1000);
  }

  const { hexFlashing } = useStatus();
  const { flashingTimeoutSeconds } = useRuntimeConfig().public;
  function flashFileToEcu() {
    hexFlashing.value = 0;
    socket.value.emit('W_Flash', 'Start');
    console.debug(`[ECU] Flashing file to ecu...`);
    addLog({ type: 'info', message: `Flashing file to ecu...` });

    // Waiting for timeout period then stop trying to verify
    setTimeout(() => {
      if (hexFlashing.value === null || hexFlashing.value === 100) return;

      hexFlashing.value = null;
      const message = `File flashing timeout reached (${flashingTimeoutSeconds}s).`;
      console.debug(`[ECU] ${message}`);
      addLog({ type: 'info', message });
    }, flashingTimeoutSeconds * 1000);
  }

  // Sending Messages
  function sendMessage(payload) {
    if (!socket.value || !socket.value.emit)
      return new Error('No socket connection.');

    socket.value.emit('message', payload);
    addLog({ type: 'sent', message: payload });
  }

  return {
    connect,
    disconnect,
    connectToCar,
    disconnectFromCar,
    connectToEcu,
    disconnectFromEcu,
    sendHexToEcu,
    flashFileToEcu,
    sendMessage,
  };
}
