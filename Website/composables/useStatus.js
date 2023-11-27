export default function () {
  // Websocket Browser Support
  const hasWebSocket = useState('websocket', () => false);

  // Socket Server Connection (0 = Disconnected, 1 = Connected, 2 = Connecting)
  const serverConnection = useState('serverConnection', () => 0);

  // Car Connection (0 = Disconnected, 1 = Connected, 2 = Connecting)
  const carConnection = useState('carConnection', () => 0);

  // ECU Connection (0 = Disconnected, 1 = Connected, 2 = Connecting) for each
  const ecuConnection = useState('ecuConnection', () => ({
    ecu1: 0,
    ecu2: 0,
    ecu3: 0,
  }));

  const isEcuConnecting = computed(() => {
    let connectingId;

    for (const [key, value] of Object.entries(ecuConnection.value)) {
      if (value === 2) {
        connectingId = Number(key.slice(-1)); // Returns the id of the ecu that's either connecting or connected
        break;
      }
      connectingId = null;
    }

    return connectingId;
  });

  const connectedEcuId = computed(() => {
    let connectedId;

    for (const [key, value] of Object.entries(ecuConnection.value)) {
      if (value === 1) {
        connectedId = Number(key.slice(-1)); // Returns the id of the ecu that's either connecting or connected
        break;
      }
      connectedId = null;
    }

    return connectedId;
  });

  // ECU Files (Verification, Flashing)
  const hexVerification = useState('isHexVerified', () => 0); // 0 = Not Verified, 1 = Verified, 2 = Verifying
  const hexFlashing = useState('isHexFlashing', () => null); // null = Not Flashing, 0 : 99 = Flashing Progress, 100 = Flashing Complete

  return {
    hasWebSocket,
    serverConnection,
    carConnection,
    ecuConnection,
    isEcuConnecting,
    connectedEcuId,
    hexVerification,
    hexFlashing,
  };
}
