<script setup>
  // Updating Websocket status state
  const { hasWebSocket } = useStatus();
  onMounted(() => {
    if ('WebSocket' in window) {
      hasWebSocket.value = true;
    } else {
      hasWebSocket.value = false;
      return new Error('Websocket is not supported in this browser');
    }
  });

  // Updating ui classes
  const socketStatusClass = computed(() => ({
    'text-red-500': hasWebSocket.value === false,
    'text-green-500': hasWebSocket.value === true,
  }));
</script>

<template>
  <AppCard>
    <div class="flex items-center gap-2">
      <StatusDot :class="socketStatusClass" />
      <p>Socket Support</p>
    </div>
  </AppCard>
</template>
