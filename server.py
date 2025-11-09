#!/usr/bin/env python3
"""
Simple HTTP Server for Sudoku Solver Master Frontend
Run this script to serve the frontend files locally
"""

import http.server
import socketserver
import os
import webbrowser
from pathlib import Path

# Configuration
PORT = 8000
DIRECTORY = Path(__file__).parent

class CustomHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(DIRECTORY), **kwargs)
    
    def end_headers(self):
        # Add CORS headers for development
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        super().end_headers()

def main():
    """Start the HTTP server and open the browser"""
    
    # Check if required files exist
    required_files = ['index.html', 'styles.css', 'script.js']
    missing_files = [f for f in required_files if not (DIRECTORY / f).exists()]
    
    if missing_files:
        print(f"❌ Missing required files: {', '.join(missing_files)}")
        print("Please ensure all frontend files are in the same directory as this script.")
        return
    
    # Create server
    with socketserver.TCPServer(("", PORT), CustomHTTPRequestHandler) as httpd:
        print("🎯 Sudoku Solver Master - Frontend Server")
        print("=" * 50)
        print(f"📁 Serving files from: {DIRECTORY}")
        print(f"🌐 Server running at: http://localhost:{PORT}")
        print(f"📱 Open your browser and navigate to: http://localhost:{PORT}")
        print("=" * 50)
        print("Press Ctrl+C to stop the server")
        print()
        
        # Try to open browser automatically
        try:
            webbrowser.open(f'http://localhost:{PORT}')
            print("✅ Browser opened automatically")
        except:
            print("⚠️  Could not open browser automatically")
            print("   Please manually open: http://localhost:{PORT}")
        
        print()
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n🛑 Server stopped by user")
            print("👋 Thank you for using Sudoku Solver Master!")

if __name__ == "__main__":
    main() 