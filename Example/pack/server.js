// server-entry
import { createApp } from './app.js'

export default context => {
  const { app } = createApp()
  return app
}