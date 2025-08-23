document.addEventListener('DOMContentLoaded', () => {
    const contentDisplay = document.getElementById('content-display');
    const navLinks = document.querySelectorAll('.sidebar .doc-links a');

    // Simple function to escape HTML for security in code blocks
    const escapeHTML = (str) => {
        return str.replace(/</g, '&lt;').replace(/>/g, '&gt;');
    };

    // A simple native JS Markdown to HTML parser
    const parseMarkdownToHTML = (markdown) => {
        const lines = markdown.split('\n');
        let html = '';
        let inCodeBlock = false;

        for (const line of lines) {
            if (line.trim().startsWith('```')) {
                if (inCodeBlock) {
                    html += '</code></pre>';
                    inCodeBlock = false;
                } else {
                    html += '<pre><code>';
                    inCodeBlock = true;
                }
                continue;
            }

            if (inCodeBlock) {
                html += escapeHTML(line) + '\n';
            } else {
                if (line.startsWith('# ')) {
                    html += `<h1>${line.substring(2)}</h1>`;
                } else if (line.startsWith('## ')) {
                    html += `<h2>${line.substring(3)}</h2>`;
                } else if (line.startsWith('### ')) {
                    html += `<h3>${line.substring(4)}</h3>`;
                } else if (line.trim() !== '') {
                    html += `<p>${line}</p>`;
                }
            }
        }
        // Close code block if file ends while still inside it
        if (inCodeBlock) {
            html += '</code></pre>';
        }
        return html;
    };

    // Function to fetch and render content
    const loadContent = async (url) => {
        try {
            contentDisplay.innerHTML = '<h2>Loading...</h2>';
            const response = await fetch(url);
            if (!response.ok) {
                throw new Error(`Failed to fetch: ${response.statusText}`);
            }
            const markdown = await response.text();
            contentDisplay.innerHTML = parseMarkdownToHTML(markdown);
        } catch (error) {
            console.error('Error loading documentation:', error);
            contentDisplay.innerHTML = `<h2>Error</h2><p>Could not load content from <code>${url}</code>. Please check the console for more details.</p>`;
        }
    };

    // Attach event listeners to all documentation links
    navLinks.forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            const url = link.getAttribute('href');
            history.pushState(null, '', `?page=${url}`); // Update URL for better UX
            loadContent(url);
        });
    });

    // Optional: Load content based on URL query parameter on page load
    const initialPage = new URLSearchParams(window.location.search).get('page');
    if (initialPage) {
        loadContent(initialPage);
    }
});
