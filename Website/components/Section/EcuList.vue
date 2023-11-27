<script setup>
  // Connecting to ECU
  const { connectToEcu, disconnectFromEcu } = useSocketClient();

  // ECU Status
  const { ecuConnection, isEcuConnecting, connectedEcuId } = useStatus();
</script>

<template>
  <section>
    <div class="app-heading mb-2">
      <UIcon name="i-fa6-solid-microchip" class="text-lg" />
      <h1>ECUs List</h1>
    </div>

    <div class="flex gap-4 [&>*]:basis-full">
      <AppCard v-for="(_, index) in 3" :key="index">
        <div class="flex items-center gap-2">
          <StatusDot
            :class="{
              'text-red-500': ecuConnection[`ecu${index + 1}`] === 0,
              'text-green-500': ecuConnection[`ecu${index + 1}`] === 1,
              'text-orange-500': ecuConnection[`ecu${index + 1}`] === 2,
            }"
          />
          <p>ECU {{ index + 1 }}</p>
        </div>

        <UButton
          v-if="ecuConnection[`ecu${index + 1}`] !== 1"
          :loading="ecuConnection[`ecu${index + 1}`] === 2"
          :disabled="
            !!isEcuConnecting ||
            (connectedEcuId && connectedEcuId !== index + 1)
          "
          @click="connectToEcu(index + 1)"
        >
          {{
            ecuConnection[`ecu${index + 1}`] === 2 ? 'Connecting' : 'Connect'
          }}
        </UButton>

        <UButton v-else variant="outline" @click="disconnectFromEcu(index + 1)">
          {{
            ecuConnection[`ecu${index + 1}`] === 2
              ? 'Disconnecting'
              : 'Disconnect'
          }}
        </UButton>
      </AppCard>
    </div>
  </section>
</template>
