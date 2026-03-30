<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { DEFAULT_SYSTEM_PROMPT } from '$lib/data/settings-constants';

  let promptValue = $state(settingsStore.current.defaultSystemPrompt);

  $effect(() => {
    promptValue = settingsStore.current.defaultSystemPrompt;
  });

  function handleInput(e: Event) {
    const target = e.target as HTMLTextAreaElement;
    promptValue = target.value;
    settingsStore.updateSystemPrompt(promptValue);
  }

  function handleReset() {
    promptValue = DEFAULT_SYSTEM_PROMPT;
    settingsStore.updateSystemPrompt(promptValue);
  }
</script>

<div>
  <h2 class="font-display text-lg text-on-bg">Systeemprompt</h2>
  <p class="font-body text-sm text-on-bg-dim mt-1">
    Deze instructie wordt bij elk gesprek naar het AI-model gestuurd.
  </p>

  <textarea
    class="mt-4 w-full h-40 bg-surface-0 rounded-xl p-4 text-on-bg font-body text-sm resize-y focus:ring-2 focus:ring-primary/20 focus:shadow-glow-sm outline-none"
    value={promptValue}
    oninput={handleInput}
  ></textarea>

  <div class="flex justify-between items-center mt-3">
    <span class="font-label text-xs text-on-bg-muted">{promptValue.length} tekens</span>
    <button
      type="button"
      class="text-on-bg-muted hover:text-on-bg text-sm font-body cursor-pointer transition-colors duration-150"
      onclick={handleReset}
    >
      Herstellen
    </button>
  </div>
</div>
