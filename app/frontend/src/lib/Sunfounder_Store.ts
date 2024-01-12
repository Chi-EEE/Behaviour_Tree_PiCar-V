import { writable, derived } from 'svelte/store'

const angle = writable(0)
const angleInRadians = derived($angle, ($angle * Math.PI) / 180)
const positionX = writable(0)
const positionY = writable(0)

export { angle, angleInRadians, positionX, positionY }