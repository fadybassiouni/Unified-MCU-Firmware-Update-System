export default defineNuxtConfig({
  modules: ['@nuxt/ui', '@nuxtjs/supabase', '@nuxtjs/google-fonts'],

  ui: { icons: 'all' },

  supabase: {
    redirect: false, // This disables the default behavior of auto redirecting to the /login page.
    url: process.env.SUPABASE_URL,
    key: process.env.SUPABASE_ANON_KEY,
  },

  googleFonts: {
    families: { 'IBM+Plex+Sans': { wght: [400, 500] } },
    display: 'swap',
    prefetch: false,
    download: true,
    outputDir: 'assets',
    stylePath: 'fonts/google-fonts.css',
  },

  runtimeConfig: {
    public: {
      socketHost: process.env.SOCKET_HOST,
      socketRoom: process.env.SOCKET_ROOM,
      fernetKey: process.env.FERNET_KEY,
      carToken: process.env.CAR_TOKEN,
      carConnectionTimeoutSeconds: process.env.CAR_CONNECTION_TIMEOUT_SECONDS,
      ecuConnectionTimeoutSeconds: process.env.ECU_CONNECTION_TIMEOUT_SECONDS,
      verificationTimeoutSeconds: process.env.VERIFICATION_TIMEOUT_SECONDS,
      flashingTimeoutSeconds: process.env.FLASHING_TIMEOUT_SECONDS,
      skipLoginRedirect: process.env.SUPABASE_SKIP_LOGIN_REDIRECT === 'true',
    },
  },
});
