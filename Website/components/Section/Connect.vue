<script setup>
  // Fetching status to toggle sections visibility
  const { hasWebSocket, serverConnection } = useStatus();

  // Server Connecting/Disconnecting & Handlers
  const { connect, disconnect, connectToCar, disconnectFromCar } =
    useSocketClient();
</script>

<template>
  <section>
    <div class="app-heading mb-2">
      <UIcon name="i-pajamas-status-health" class="text-lg" />
      <h1>Connection Status</h1>
    </div>

    <div class="flex gap-4 flex-col" v-auto-animate>
      <ConnectionWebSocket />
      <ConnectionServer
        v-if="hasWebSocket"
        @connect="(socketHost) => connect(socketHost)"
        @disconnect="disconnect"
      />
      <ConnectionCar
        @connect="(carToken) => connectToCar(carToken)"
        @disconnect="disconnectFromCar"
        v-if="serverConnection === 1"
      />
    </div>
  </section>
</template>
