<script setup>
  // Redirecting to home page if user is logged in
  const user = useSupabaseUser();
  const { skipLoginRedirect } = useRuntimeConfig().public;
  onMounted(() => {
    if (skipLoginRedirect) return; // Bypass auth checking if set from the env
    if (user.value) return navigateTo('/');
  });

  // Validating the form
  const state = ref({ email: undefined, password: undefined });
  const validate = (state) => {
    const errors = [];

    // Validating Email
    const emailRegex = /^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$/i;
    if (!state.email) errors.push({ path: 'email', message: 'Required' });
    else if (!emailRegex.test(state.email))
      errors.push({ path: 'email', message: 'Invalid email' });

    // Validating Password
    if (!state.password) errors.push({ path: 'password', message: 'Required' });

    return errors;
  };

  // Submitting the form
  const loginLoading = ref(false);
  const supabase = useSupabaseClient();
  const toast = useToast();
  async function login() {
    loginLoading.value = true;

    const { error } = await supabase.auth.signInWithPassword({
      email: state.value.email,
      password: state.value.password,
    });

    if (error) {
      state.value.email = undefined;
      state.value.password = undefined;
      toast.add({ title: 'Invalid credentials! Please try again.' });
      loginLoading.value = false;
      return;
    }

    toast.add({ title: 'Logged in successfully!' });
    loginLoading.value = false;
    return navigateTo('/');
  }
</script>

<template>
  <UContainer class="grid place-content-center gap-10">
    <div class="text-center">
      <h1 class="font-medium text-lg">Welcome Back</h1>
      <p class="app-caption">You have been missed!</p>
    </div>

    <UForm
      :state="state"
      :validate="validate"
      @submit="login"
      class="sm:min-w-[340px] flex flex-col justify-center gap-4"
    >
      <UFormGroup label="Email" name="email">
        <UInput v-model="state.email" />
      </UFormGroup>

      <UFormGroup label="Password" name="password">
        <UInput v-model="state.password" type="password" />
      </UFormGroup>

      <UButton :loading="loginLoading" type="submit" class="justify-center">
        Login
      </UButton>
    </UForm>
  </UContainer>
</template>
