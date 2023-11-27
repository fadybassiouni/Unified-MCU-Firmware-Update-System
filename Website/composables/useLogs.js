export default function () {
  const logs = useState('logs', () => []);

  function addLog(newLog) {
    logs.value.push({ id: new Date().getTime(), ...newLog });
  }

  function clearLogs() {
    logs.value = [];
  }

  return { logs, addLog, clearLogs };
}
