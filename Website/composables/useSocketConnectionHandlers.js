export default function (socket) {
  const { addLog, clearLogs } = useLogs();

  // Server
  const { serverConnection } = useStatus();
  socket.value.on('connect', serverConnect);
  function serverConnect() {
    console.debug('[connect] Connected');
    addLog({ type: 'success', message: 'Connected to server successfully!' });
    serverConnection.value = 1;
  }

  socket.value.on('disconnect', serverDisconnect);
  function serverDisconnect() {
    console.debug('[disconnect] Disconnected');
    socket.value.disconnect();
    clearLogs();

    // Resetting UI state
    serverConnection.value = 0;
    carConnection.value = 0;
    ecuConnection.value.ecu1 = 0;
    ecuConnection.value.ecu2 = 0;
    ecuConnection.value.ecu3 = 0;
  }

  socket.value.on('close', socketClose);
  function socketClose(payload) {
    console.debug('[close] Close:', payload);
  }

  socket.value.on('error', socketError);
  function socketError(error) {
    console.debug('[error] Error:', error);
  }

  socket.value.on('connect_error', serverConnectionError);
  const connectionRetries = ref(1);
  function serverConnectionError(error) {
    // Retries connection 3 times before closing connection
    console.debug(
      `[connect_error] Connection Error (Attempt ${connectionRetries.value}/3):`,
      error.context,
    );
    addLog({
      type: 'warning',
      message: `Error connecting to server (Attempt ${connectionRetries.value}/3)`,
    });
    connectionRetries.value++;

    // Closing connection and notifying user
    if (connectionRetries.value <= 3) return;
    serverDisconnect();
  }

  // Car
  const { carConnection } = useStatus();
  socket.value.on('W_connectCar', carConnect);
  function carConnect(payload) {
    console.debug('[W_connectCar] Response:', payload);

    // Updating `carConnection` status
    if (payload === 'Connection Accepted') {
      carConnection.value = 1;
      addLog({ type: 'success', message: 'Connected to car successfully!' });
    } else if (payload === 'Bad Token') {
      carConnection.value = 0;
      addLog({ type: 'error', message: 'Error connecting to car!' });
    } else {
      addLog({ type: 'error', message: 'Unknown car response on connect!' });
    }
  }

  socket.value.on('W_disconnectCar', carDisconnect);
  function carDisconnect() {
    console.debug('[W_disconnectCar] Response');

    // Updating `carConnection` status
    carConnection.value = 0;
    addLog({ type: 'success', message: 'Disconnected from car successfully!' });
  }

  // ECU
  const { ecuConnection } = useStatus();
  socket.value.on('W_connectECU', ecuConnect);
  function ecuConnect(payload) {
    console.debug('[W_connectECU] Response:', payload);

    // Updating `ecuConnection` status
    if (payload.includes('Connection Established With ECU')) {
      const ecuId = payload.slice(-1);
      ecuConnection.value[`ecu${ecuId}`] = 1;
      addLog({
        type: 'success',
        message: `Connected to ECU${ecuId} successfully!`,
      });
    } else if (payload.includes('Invalid ECU ID')) {
      ecuConnection.value.ecu1 = 0;
      ecuConnection.value.ecu2 = 0;
      ecuConnection.value.ecu3 = 0;
      addLog({ type: 'error', message: 'Error connecting to ECU!' });
    } else if (payload.includes('Invalid ECU ID')) {
      addLog({ type: 'error', message: 'Invalid ECU ID!' });
    } else {
      addLog({ type: 'error', message: 'Unknown ECU response on connect!' });
    }
  }

  socket.value.on('W_disconnectECU', ecuDisconnect);
  function ecuDisconnect() {
    console.debug('[W_disconnectECU] Response');

    // Updating `ecuConnection` status
    ecuConnection.value.ecu1 = 0;
    ecuConnection.value.ecu2 = 0;
    ecuConnection.value.ecu3 = 0;
    addLog({ type: 'success', message: 'Disconnected from ecu successfully!' });
  }

  const { hexVerification } = useStatus();
  socket.value.on('W_receiveHex', ecuFileVerification);
  function ecuFileVerification(payload) {
    console.debug('[W_receiveHex] Response', payload);

    if (payload === 'OK') {
      hexVerification.value = 1;
      addLog({
        type: 'success',
        message: 'Server verified the hex file successfully!',
      });
    } else if (payload === 'NOK') {
      hexVerification.value = 0;
      addLog({
        type: 'error',
        message: 'Server could not verify the hex file.',
      });
    }
  }

  const { hexFlashing } = useStatus();
  socket.value.on('W_progress', ecuMasterFlashing);
  function ecuMasterFlashing(payload) {
    console.debug('[W_progress] Response', payload);
    hexFlashing.value = Number(payload);

    if (Number(payload) === 100)
      addLog({ type: 'success', message: 'ECU flashed successfully!' });
  }

  socket.value.on('W_Flash', ecuSlaveFlashing);
  function ecuSlaveFlashing(payload) {
    console.debug('[W_Flash] Response', payload);
    addLog({ type: 'received', message: payload });
  }

  // Returning socket handlers (may be used for debugging)
  return {
    serverConnect,
    serverDisconnect,
    socketClose,
    socketError,
    serverConnectionError,
    carConnect,
    ecuConnect,
    ecuDisconnect,
    carDisconnect,
    ecuMasterFlashing,
    ecuSlaveFlashing,
  };
}
