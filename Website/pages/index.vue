<script setup>
  // Redirecting to login page if user is not logged in
  // const user = useSupabaseUser();
  // const { skipLoginRedirect } = useRuntimeConfig().public;
  // onMounted(() => {
  //   if (skipLoginRedirect) return; // Bypass auth checking if set from the env
  //   if (!user.value) return navigateTo('/login');
  // });

  // Toggling sections visibility based on connection status
  const { carConnection, connectedEcuId } = useStatus();
</script>

<template>
  <UContainer v-auto-animate class="py-4 flex flex-col gap-8 lg:min-w-[54rem]">
    <SectionUserCard />
    <SectionConnect />
    <SectionEcuList v-if="carConnection === 1" />
    <SectionSoftwareUpdate v-if="connectedEcuId" />
    <SectionLogs />
  </UContainer>
</template>
