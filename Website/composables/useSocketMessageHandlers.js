export default function (socket) {
  const { addLog } = useLogs();

  function message(payload) {
    console.debug('[Socket] Message');
    addLog({ type: 'received', message: payload });
  }

  // Passing all of the above to socket instance
  socket.value.on('message', message);

  // Returning socket handlers (may be used for debugging)
  return {
    message,
  };
}
